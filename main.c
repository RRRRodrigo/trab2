
#include <imageprocessing.h>
#include <stdio.h>


int main() {
  imagem img, img_copy;

  img = abrir_imagem("data/cachorro.jpg");
  img_copy = abrir_imagem("data/cachorro.jpg");

  unsigned int tmp;
  unsigned int cntR,
               cntG,
               cntB,
               cntArea;
  int r;

  printf("\nDigite o raio do blur: \n");
  scanf("%d", &r);

  for (int i=0; i<(img.width); i++) {
    for (int j=0; j<(img.height); j++) {
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
            cntR += img.r[(j+l)*img.width + i+k];
            cntG += img.g[(j+l)*img.width + i+k];
            cntB += img.b[(j+l)*img.width + i+k];

            cntArea++; 
          }
        }

      }
       //gera o pixel da nova imagem
      img_copy.r[j*img.width + i] = cntR/cntArea;         
      img_copy.g[j*img.width + i] = cntG/cntArea; 
      img_copy.b[j*img.width + i] = cntB/cntArea; 


    }
  }

  salvar_imagem("cachorro-out.jpg", &img_copy);
  liberar_imagem(&img);
  liberar_imagem(&img_copy);
  return 0;
}
