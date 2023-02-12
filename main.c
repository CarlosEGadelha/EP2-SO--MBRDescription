#include <stdio.h>
#include <string.h>
#include <stdint.h>

static uint8_t c[512];

static char * conversor_para_memoria(uint32_t size) 
{
	double tm_kb, tm_mb, tm_gb;
	char *tm_str = malloc(10);
	tm_kb = size / 2;
	tm_mb = tm_kb / 1024;
	tm_gb = tm_mb / 1024;
	if (tm_gb>=1)
		sprintf(tm_str, "%.2fGB", tm_gb);
	else if (tm_mb>=1)
		sprintf(tm_str, "%.2fMB", tm_mb);
	else 
		sprintf(tm_str, "%.2fKB", tm_kb);
	return tm_str;
}

static void comeco_particao(uint32_t start) 
{
	uint8_t *start_part = &c[start];
	uint32_t start_dec = *(uint32_t *)start_part;
	printf("\nPARTIÇÃO INICIA EM %u\n", start_dec);
}


static void final_particao(uint32_t end, uint32_t start) 
{
	uint8_t *start_part = &c[start];
	uint32_t start_dec = *(uint32_t *)start_part;
	uint8_t *end_part = &c[end];
	uint32_t end_dec = *(uint32_t *)end_part;
  uint32_t fim = end_dec + start_dec;
	printf("\nPARTIÇÃO TERMINA EM %u\n", fim);
}

static void total(uint32_t end)
{
	uint8_t *end_part = &c[end];
	uint32_t end_dec = *(uint32_t *)end_part;
	printf("\nTAMANHO TOTAL DA PARTIÇÃO %u\n", end_dec);
}

int main(int argc, char **argv)
{
  FILE *f;
  
  f = fopen(argv[1], "r");
  fseek(f, 0, SEEK_SET);
  fread(c,512,1, f);

  if(c[510]==0x55 && c[511]==0xaa){
    printf("\nA ASSINATURA ACEITA \n"); 
  }
  else{
    printf("A ASSINATURA NÃO FOI ACEITA \n");  
    fclose(f);
    return 0;
  }

  for(int i=1; i<=4; i++)
  {
    printf("\n----------------------------------------");
    
    int ini_particao = 430+(16*i);
    int total_particao = 442+(16*i);
    int id_particao = 434+(16*i);
    printf("\nPARTIÇÃO %d:\n",i);
    if(c[ini_particao] == 0x80)
    {
      printf("\nSTATUS: ATIVO\n");
      
      comeco_particao(ini_particao);

      final_particao(total_particao, ini_particao);
      
      total(total_particao);

      printf("\nTAMANHO:\n");
      uint8_t *start_part = &c[total_particao];
	    uint32_t start_dec = *(uint32_t *)start_part;
      printf("%s\n",conversor_para_memoria(start_dec));

      printf("\nID\n");
      uint8_t id = c[id_particao];
      printf("%x",id);

      printf("\nTIPO\n");
      if(c[id_particao] == 0x83)
        printf("LINUX");
      if(c[id_particao] == 0x82)
        printf("LINUX SWAP");
      if(c[id_particao] == 0x42)
        printf("WINDOWS DYNAMIC");
      
    }
    else{
      printf("\nSTATUS: PARTIÇÃO NÃO BOOTAVEL\n");
    }
    
    printf("\n----------------------------------------");
  }

  fclose(f);

  return 0;
}
