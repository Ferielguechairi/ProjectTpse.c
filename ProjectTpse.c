Question 1

Les structures de données à utiliser sont les suivantes :

Une matrice B de taille n1 * m1, contenant les valeurs aléatoires de la première matrice.
Une matrice C de taille n2 * m2, contenant les valeurs aléatoires de la deuxième matrice.
Un tampon T de taille N, contenant les résultats intermédiaires du produit des matrices.
Question 2

Pour protéger l'accès aux données, il est nécessaire d'utiliser des mutex ou des sémaphores. Les mutex permettent de protéger l'accès à une ressource unique, tandis que les sémaphores permettent de contrôler le nombre de threads qui peuvent accéder à une ressource.

Dans ce cas, il est nécessaire de protéger l'accès au tampon T. Les producteurs doivent pouvoir accéder au tampon pour y placer les résultats intermédiaires, tandis que les consommateurs doivent pouvoir y accéder pour les consommer.

Une solution possible consiste à utiliser deux mutex :

Un mutex pour protéger l'accès à l'en-tête du tampon. Ce mutex est utilisé par les producteurs pour vérifier si le tampon est plein avant d'y placer un résultat.
Un mutex pour protéger l'accès à la queue du tampon. Ce mutex est utilisé par les consommateurs pour vérifier si le tampon est vide avant de consommer un résultat.
Une autre solution consiste à utiliser un sémaphore :

Un sémaphore pour contrôler le nombre de résultats disponibles dans le tampon. Ce sémaphore est utilisé par les producteurs pour decrementer le nombre de résultats disponibles avant d'y placer un résultat. Il est utilisé par les consommateurs pour incrementer le nombre de résultats disponibles avant de consommer un résultat.
Question 3

Les risques liés à ce type de solution sont les suivants :

La concurrence entre les producteurs et les consommateurs. Si les producteurs et les consommateurs tentent d'accéder au tampon au même moment, il peut y avoir des conflits.
La corruption des données. Si un thread accède au tampon alors qu'il est dans un état incohérent, les données peuvent être corrompues.
Pour réduire ces risques, il est important de bien synchroniser l'accès aux données. Les mutex et les sémaphores sont des outils efficaces pour la synchronisation des threads.

En plus de ces risques, il est également important de prendre en compte les performances du système. Le modèle producteur/consommateur peut être efficace pour les tâches qui nécessitent un traitement en parallèle, mais il peut également avoir un impact négatif sur les performances si le tampon est trop petit ou si la synchronisation des threads est mal gérée.

le programme :
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define n1 3
#define m1 3 
#define n2 3 
#define m2 3 
#define N (n1 * m2) // Taille du tampon

int B[n1][m1];
int C[n2][m2];
int A[n1][m2];
int T[N];
/*vous qui enter les valeure de matrice B et C*/
void *producer(void *arg) {
    int row = *((int *)arg);

    for (int j = 0; j < m2; j++) {
        int result = 0;
        for (int k = 0; k < m1; k++) {
            result += B[row][k] * C[k][j];
        }
        T[row * m2 + j] = result;
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int index = *((int *)arg);
    int row = index / m2;
    int col = index % m2;

    A[row][col] = T[index];

    pthread_exit(NULL);
}

void print_matrix(char *matrix_name, int rows, int cols, int matrix[rows][cols]) {
    printf("%s :\n", matrix_name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Entrez les valeurs pour la matrice :\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            scanf("%d", &matrix[i][j]);
        }
    }
}

int main() {
    printf("Lecture de la matrice B :\n");
    read_matrix(n1, m1, B);

    printf("\nLecture de la matrice C :\n");
    read_matrix(n2, m2, C);

    printf("\n*******Matrice B*******:\n");
    print_matrix("Matrice B", n1, m1, B);

    printf("\n******Matrice C****** :\n");
    print_matrix("Matrice C", n2, m2, C);

     pthread_t producers[n1], consumers[N];

    for (int i = 0; i < n1; i++) {
        pthread_create(&producers[i], NULL, producer, &i);
    }

    for (int i = 0; i < N; i++) {
        pthread_create(&consumers[i], NULL, consumer, &i);
    }

    for (int i = 0; i < n1; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Affichage du contenu du tampon T après le remplissage
    printf("\nContenu du tampon T après le remplissage :\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", T[i]);
    }
    printf("\n");

    // Affichage de la matrice résultante A
    printf("\nMatrice résultante A :\n");
    print_matrix("Matrice résultante A", n1, m2, A);

    getchar();
    getchar();

    return 0;
}


