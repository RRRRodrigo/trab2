#include <pthread.h>
#include <imageprocessing.h>
#include <stdio.h>
#include <time.h>

int r;
imagem img, img_copy;



void * thread_R(){
  float cntR;
  float cntAreaR;

  for (int i=0; i<(img.width); i++) {
    for (int j=0; j<(img.height); j++) {
      //inicializa variaveis auxiliares
      cntR=0;
      cntAreaR=0;

      //Percorre a area de aplicacao do blur
      for(int k=-r; k<=r; k++){
        for(int l=-r; l<=r; l++){

          if((j+l)>0 && (j+l)<img.height && (i+k)>0 && (i+k)<img.width){
            //Acessa uma posicao valida e acumula os valores de cada canal dentro da area
            cntR += img.r[(j+l)*img.width + i+k];

            cntAreaR = cntAreaR + 1.0;
          }
        }

      }
       //gera o pixel da nova imagem
      img_copy.r[j*img.width + i] =((float)cntR)/((float)cntAreaR);
    }
  }
}

void * thread_G(){
  float cntG;
  float cntAreaG;
  for (int i=0; i<(img.width); i++) {
    for (int j=0; j<(img.height); j++) {
      //inicializa variaveis auxiliares
      cntG=0;
      cntAreaG=0;

      //Percorre a area de aplicacao do blur
      for(int k=-r; k<=r; k++){
        for(int l=-r; l<=r; l++){

          if((j+l)>0 && (j+l)<img.height && (i+k)>0 && (i+k)<img.width){
            //Acessa uma posicao valida e acumula os valores de cada canal dentro da area
            cntG += img.g[(j+l)*img.width + i+k];

            cntAreaG = cntAreaG + 1.0;
          }
        }

      }
       //gera o pixel da nova imagem
      img_copy.g[j*img.width + i] =((float)cntG)/((float)cntAreaG);
    }
  }
}

void * thread_B(){
  float cntAreaB;
  float cntB;
  for (int i=0; i<(img.width); i++) {
    for (int j=0; j<(img.height); j++) {
      //inicializa variaveis auxiliares
      cntB=0;
      cntAreaB=0;

      //Percorre a area de aplicacao do blur
      for(int k=-r; k<=r; k++){
        for(int l=-r; l<=r; l++){

          if((j+l)>0 && (j+l)<img.height && (i+k)>0 && (i+k)<img.width){
            //Acessa uma posicao valida e acumula os valores de cada canal dentro da area
            cntB += img.b[(j+l)*img.width + i+k];

            cntAreaB = cntAreaB + 1.0;
          }
        }

      }
       //gera o pixel da nova imagem
      img_copy.b[j*img.width + i] =((float)cntB)/((float)cntAreaB);
    }
  }
}


int main(){
  clock_t t0, t1;
  t0=clock();
  pthread_t workers[3];
  img = abrir_imagem("data/cachorro.jpg");
  img_copy = abrir_imagem("data/cachorro.jpg");


  printf("\nDigite o raio do blur: \n");
  scanf("%d", &r);

  pthread_create(&(workers[0]), NULL, thread_R, NULL);
  pthread_create(&(workers[1]), NULL, thread_G, NULL);
  pthread_create(&(workers[2]), NULL, thread_B, NULL);

  pthread_join(workers[0], NULL);
  pthread_join(workers[1], NULL);
  pthread_join(workers[2], NULL);

  salvar_imagem("cachorro-out.jpg", &img_copy);
  liberar_imagem(&img);
  liberar_imagem(&img_copy);
  t1=clock();
  printf("Tempo para aplicar blur: %lf segundos\n", (double)(t1-t0)/CLOCKS_PER_SEC);
  return 0;
}
