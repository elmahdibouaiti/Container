#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>
#include <sys/mount.h>
#include <sys/resource.h>

#define STACK_SIZE (1024 * 1024)

static char container_stack[STACK_SIZE];

void container_code(void *arg) {
    printf("Démarrage du conteneur - PID du conteneur : %d\n", getpid());

    // Limiter les ressources du conteneur
    struct rlimit rlim;
    rlim.rlim_cur = 3000000;  // Limite de la mémoire virtuelle
    rlim.rlim_max = 3000000;  // Limite de la mémoire physique
    setrlimit(RLIMIT_AS, &rlim);

    // Montage minimal de système de fichiers
    mount("none", "/", NULL, MS_REC|MS_PRIVATE, NULL);
    mount("none", "/tmp", "tmpfs", 0, NULL);

    // Montage "bind" pour partager un dossier avec l'extérieur dans le conteneur
    mount("/host_folder", "/tmp/host_folder", NULL, MS_BIND, NULL);

    // Changer le répertoire racine pour le conteneur
    chroot("/tmp");
    chdir("/");

    // Isoler l'interface réseau du conteneur (nécessite des privilèges)
    unshare(CLONE_NEWNET);

    // Restreindre les capabilities
    cap_t caps;
    caps = cap_get_proc();
    cap_clear(caps);
    cap_set_flag(caps, CAP_PERMITTED, 1, CAP_SYS_ADMIN, CAP_SETUID, CAP_SETGID, CAP_SETPCAP, CAP_SYS_PTRACE, -1);
    cap_set_flag(caps, CAP_EFFECTIVE, 1, CAP_SYS_ADMIN, CAP_SETUID, CAP_SETGID, CAP_SETPCAP, CAP_SYS_PTRACE, -1);
    cap_set_proc(caps);
    cap_free(caps);

    // Exécuter un shell dans le conteneur
    execlp("/bin/bash", "/bin/bash", NULL);
}

int main() {
    pid_t pid;

    printf("Démarrage du processus principal - PID : %d\n", getpid());

    // Création d'un nouveau namespace pour le conteneur
    pid = clone(container_code, container_stack + STACK_SIZE, CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);

    if (pid == -1) {
        perror("clone");
        exit(EXIT_FAILURE);
    }

    // Attendre la fin du conteneur
    waitpid(pid, NULL, 0);

    printf("Fin du conteneur\n");

    return 0;
}
