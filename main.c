#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <setjmp.h>
#include <math.h> //floor

#define LINE_W 4 //width of rects being sorted
#define WIN_H 480
#define WIN_W 800
SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
TTF_Font *FONT = NULL;

void render(int *, int);

#include "sort.c"

typedef void (*sort_fun)(int, int*);
sort_fun sort_funcs[] = {
  &quickSort, &insertionSort, &selectionSort,
  &bubbleSort, &cycleSort, &countSort,
  &flashSort, &oddEvenSort, &mergeSort,
  &shellSort, &combSort, &gnomeSort,
  &stoogeSort, &monkeySort
};

const char * name_sort_funcs[] = {
  "quickSort", "insertionSort", "selectionSort",
  "bubbleSort", "cycleSort", "countSort",
  "flashSort", "oddEvenSort", "mergeSort",
  "shellSort", "combSort", "gnomeSort",
  "stoogeSort", "monkeySort"
};

int gSortInd = 0;

// this allows me to skip to next sort func even though render loop is called 
// from inside sort function itself
// particularly needed for the joke sort functions
sigjmp_buf resume_here; 

/////////////////////////////////////////
int quit(void)
{
  SDL_DestroyWindow( window );
  TTF_CloseFont(FONT);
  FONT = NULL;
  SDL_Quit();
  exit(EXIT_SUCCESS);
}

void all_vals_out_of_order(int max_val, int arr_len, int *arr)
{
  for(int i=0;i<arr_len;i++) arr[i] = (i * max_val) / arr_len;
  for(int i=0;i<arr_len;i++) swap(&arr[rand()%arr_len], &arr[i]);
}

void random_vals(int max_val, int arr_len, int *arr)
{
  for(int i=0;i<arr_len;i++) arr[i] = ((float)rand()/RAND_MAX) * max_val;
}

void render(int *arr, int delay)
{
  SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xFF);
  SDL_RenderClear(renderer);

  //render funcs
  int n = WIN_W/LINE_W;
  //SDL_SetRenderDrawColor(renderer, 0xFF,0x33,0x22,0xFF);
  for(int i=0; i < n; i++){
    SDL_SetRenderDrawColor(renderer, (arr[i] * 0xFF) / WIN_H, 0x33, 0x22, 0xFF);
    SDL_Rect rect = {i*LINE_W, WIN_H-arr[i], LINE_W, arr[i]};
    SDL_RenderFillRect(renderer, &rect);
  }

  //render text
  SDL_Surface *ts = NULL;
  ts = TTF_RenderText_Blended(FONT, 
      name_sort_funcs[gSortInd], 
      (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF});

  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, ts);
  if(!t) return;
  SDL_Rect tmp = {0, 0, ts->w, ts->h};
  SDL_FreeSurface(ts);

  SDL_RenderCopy(renderer, t, NULL, &tmp);
  SDL_DestroyTexture(t);

  SDL_RenderPresent(renderer);

  //check for event
  SDL_Event ev;
  SDL_WaitEventTimeout(&ev, delay);
  switch(ev.type){
    case SDL_QUIT:
      quit();
      break;
    case SDL_KEYUP:
      switch(ev.key.keysym.sym){
        case SDLK_ESCAPE:
        case SDLK_q:
          quit();
          break;
        default: //next
          siglongjmp(resume_here, 1);
          break;
      }
      break;
  }

}

//needs to be a seperate function because jmp messes with stack, returning cleans it up
void run_loop(int arr_len, int* array)
{
  random_vals(WIN_H, arr_len, array);

  if(!sigsetjmp(resume_here, 1)){ 
    (*sort_funcs[gSortInd])(arr_len, array); 

    //wait for event before exiting
    SDL_Event ev;
    while(1){
      SDL_WaitEvent(&ev);
      switch(ev.type){
        case SDL_QUIT:
          quit();
          break;
        case SDL_KEYUP:
          switch(ev.key.keysym.sym){
            case SDLK_ESCAPE:
            case SDLK_q:
              quit();
              break;
            default: //next
              return;
              break;
          }
          break;
      }
    }
  }
  printf("skip\n"); //TODO determine why this is needed to prevent skipping multiple sorts
  return;
}

int main(void)
{
  srand(time(NULL));
  if(SDL_Init(SDL_INIT_VIDEO) < 0) exit(EXIT_FAILURE);

  if( TTF_Init() == -1) exit(EXIT_FAILURE);

  SDL_CreateWindowAndRenderer(WIN_W,WIN_H,0,&window,&renderer);

  
  SDL_SetWindowTitle(window, "Vis Sorter");

  FONT = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono.ttf", 12);

  if(FONT == NULL) exit(EXIT_FAILURE);

  int arr_len = WIN_W/LINE_W;
  int array[arr_len];

  while(1){ 
    run_loop(arr_len, array);
    //fprintf(stderr, "%i\n", gSortInd);
    gSortInd = (gSortInd+1)%14;
  }
}
