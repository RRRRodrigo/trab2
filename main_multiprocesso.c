#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <imageprocessing.h>
#include <stdio.h>

void thread_R(imagem img, imagem img_copy){
  for (int i=0; i<(img->width); i++) {
    for (int j=0; j<(img->height); j++) {
      //inicializa variaveis auxiliares
      cntR=0;
      cntG=0;
      cntB=0;
      cntArea=0;

      //Percorre a area de aplicacao do blur
      for(int k=-r; k<=r; k++){
        for(int l=-r; l<=r; l++){

          if((j+l)>0 && (j+l)<img.height && (i+k)>0 && (i+k)<img.width){
            //Acessa uma posicao valida e acumula os valores de cada canal dentro da area
            cntR += img->r[(j+l)*img.width + i+k];

            cntArea++;
          }
        }

      }
       //gera o pixel da nova imagem
      img_copy->r[j*img.width + i] = cntR/cntArea;
    }
  }
}

void thread_G(imagem img, imagem img_copy){
  for (int i=0; i<(img->width); i++) {
    for (int j=0; j<(img->height); j++) {
      //inicializa variaveis auxiliares
      cntR=0;
      cntG=0;
      cntB=0;
      cntArea=0;

      //Percorre a area de aplicacao do blur
      for(int k=-r; k<=r; k++){
        for(int l=-r; l<=r; l++){

          if((j+l)>0 && (j+l)<img.height && (i+k)>0 && (i+k)<img.width){
            //Acessa uma posicao valida e acumula os valores de cada canal dentro da area
            cntR += img->g[(j+l)*img.width + i+k];

            cntArea++;
          }
        }

      }
       //gera o pixel da nova imagem
      img_copy->g[j*img.width + i] = cntR/cntArea;
    }
  }
}

void thread_B(imagem img, imagem img_copy){
  for (int i=0; i<(img->width); i++) {
    for (int j=0; j<(img->height); j++) {
      //inicializa variaveis auxiliares
      cntR=0;
      cntG=0;
      cntB=0;
      cntArea=0;

      //Percorre a area de aplicacao do blur
      for(int k=-r; k<=r; k++){
        for(int l=-r; l<=r; l++){

          if((j+l)>0 && (j+l)<img.height && (i+k)>0 && (i+k)<img.width){
            //Acessa uma posicao valida e acumula os valores de cada canal dentro da area
            cntR += img->b[(j+l)*img.width + i+k];

            cntArea++;
          }
        }

      }
       //gera o pixel da nova imagem
      img_copy->b[j*img.width + i] = cntR/cntArea;
    }
  }
}

int r;
imagem img, img_copy;

int main(){
  int prot = PROT_READ | PROT_WRITE;
  int visi = MAP_PRIVATE | MAP_ANON;
  imagem* img = mmap(NULL, sizeof(imagem), prot, visi ,0,0);
  imagem* img_copy = mmap(NULL, sizeof(imagem), prot, visi ,0,0);

  pid_t pid[3];

  img = abrir_imagem("data/cachorro.jpg");
  img_copy = abrir_imagem("data/cachorro.jpg");

  unsigned int tmp;
  unsigned int cntR,
               cntG,
               cntB,
               cntArea;

  printf("\nDigite o raio do blur: \n");
  scanf("%d", &r);

  pid[0] = fork();
  if(!pid[0]){
    thread_R(*img, *img_copy);
    exit(0);
  }
  pid[1] = fork();
  if(!pid[1]){
    thread_G(*img, *img_copy);
    exit(0);
  }
  pid[2] = fork();
  if(!pid[2]){
    thread_B(*img, *img_copy);
    exit(0);
  }

  for(i=0;i<=2;i++)
    waitpid(-1, NULL, 0);

  salvar_imagem("cachorro-out.jpg", &img_copy);
  liberar_imagem(&img);
  liberar_imagem(&img_copy);
  return 0;
}
