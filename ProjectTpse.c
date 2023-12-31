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
#include<stdlib.h>
#include <pthread.h>

#define N 100

int B[N][N];
int C[N][N];
int A[N][N];

int T[N];

pthread_mutex_t mutex;
pthread_cond_t condition;

void *producteur(void *arg) {
  int i = *(int *)arg;
  for (int j = 0; j < N; j++) {
    // Calculer le résultat de la ligne i, colonne j de la matrice résultante A
    int resultat = 0;
    for (int k = 0; k < N; k++) {
      resultat += B[i][k] * C[k][j];
    }

    // Placer le résultat dans le tampon T
    pthread_mutex_lock(&mutex);
    T[i * N + j] = resultat;
    pthread_mutex_unlock(&mutex);

    // Signaler à un consommateur disponible qu'il y a un élément disponible
    pthread_cond_signal(&condition);
  }
}

void *consommateur(void *arg) {
  while (1) {
    // Attendre qu'un élément soit disponible dans le tampon T
    pthread_mutex_lock(&mutex);
    if (T[0] == 0) {
      pthread_cond_wait(&condition, NULL);
    }
    int resultat = T[0];
    pthread_mutex_unlock(&mutex);

    // Placer le résultat dans la matrice résultante A
    A[resultat / N][resultat % N] = resultat;
  }
}

int main() {
  // Initialiser les matrices B et C
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      B[i][j] = rand() % 100;
      C[i][j] = rand() % 100;
    }
  }

  // Initialiser le tampon T
  for (int i = 0; i < N; i++) {
    T[i] = 0;
  }

  // Créer les threads producteurs
  pthread_t producteurs[N];
  for (int i = 0; i < N; i++) {
    pthread_create(&producteurs[i], NULL, producteur, &i);
  }

  // Créer les threads consommateurs
  pthread_t consommateurs[N];
  for (int i = 0; i < N; i++) {
    pthread_create(&consommateurs[i], NULL, consommateur, NULL);
  }

  // Attendre la fin des threads
  for (int i = 0; i < N; i++) {
    pthread_join(producteurs[i], NULL);
    pthread_join(consommateurs[i], NULL);
  }

  // Afficher la matrice résultante A
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d ", A[i][j]);
    }
    printf("\n");
  }

  return 0;
}
