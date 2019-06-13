#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <imageprocessing.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int r;
imagem *img, *img_copy;

void thread_R(imagem img, imagem img_copy){
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

void thread_G(imagem img, imagem img_copy){
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

void thread_B(imagem img, imagem img_copy){
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
  struct timespec t0, t1;
  clock_gettime(CLOCK_MONOTONIC, &t0);
  int prot = PROT_READ | PROT_WRITE;
  int visi = MAP_SHARED | MAP_ANON;
  imagem* img = mmap(NULL, sizeof(imagem), prot, visi , -1,0);
  imagem* img_copy = mmap(NULL, sizeof(imagem), prot, visi , -1,0);

  pid_t pid;

  *img = abrir_imagem("data/cachorro.jpg");
  img_copy->r = mmap(NULL, (sizeof(float)*img->width*img->height), prot, visi , -1,0);
  img_copy->g = mmap(NULL, (sizeof(float)*img->width*img->height), prot, visi , -1,0);
  img_copy->b = mmap(NULL, (sizeof(float)*img->width*img->height), prot, visi , -1,0);

  img_copy->height = img->height;
  img_copy->width = img->width;


  printf("\nDigite o raio do blur: \n");
  scanf("%d", &r);

  pid = fork();
  if(!pid){
    thread_R(*img, *img_copy);
    exit(0);
  }
  pid = fork();
  if(!pid){
    thread_G(*img, *img_copy);
    exit(0);
  }
  pid = fork();
  if(!pid){
    thread_B(*img, *img_copy);
    exit(0);
  }

  for(int i=0;i<3;i++)
    waitpid(-1, NULL, 0);

  salvar_imagem("cachorro-out.jpg", img_copy);
  liberar_imagem(img);
  //FAZER UNMAP 
  munmap(img->r, (sizeof(float)*img->width*img->height));
  munmap(img->g, (sizeof(float)*img->width*img->height));
  munmap(img->b, (sizeof(float)*img->width*img->height));

  munmap(img, sizeof(imagem));
  munmap(img_copy, sizeof(imagem));
  //liberar_imagem(img_copy);
  clock_gettime(CLOCK_MONOTONIC, &t1);
  double elapsed = (t1.tv_sec - t0.tv_sec);
  elapsed += (t1.tv_nsec - t0.tv_nsec) / 1000000000.0;
  printf("Tempo para aplicar blur: %lf segundos\n", elapsed);
  return 0;
}
