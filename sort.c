/**
 Actual sorting algorithms.
*/

void swap(int *a, int *b){ int temp = *a; *a = *b; *b = temp; }

/** Best: n, Worst: n^2, Avg: n^2 */
void insertionSort(int arr_len, int *arr)
{
  int j, x;
  for(int i = 1; i < arr_len; i++){
    x = arr[i];
    j = i-1;
    while(j >= 0 && arr[j] > x){
      arr[j+1] = arr[j];
      j--;
      render(arr, 1);
    }
    arr[j+1] = x;
    render(arr, 1);
  }
}

/** Best: n^2, Worst: n^2, Avg: n^2 */
void selectionSort(int arr_len, int *arr)
{
  int i, j, min;
  for(i=0; i<arr_len-1; i++){
    min = i;
    for(j = i+1; j<arr_len; j++){
      if(arr[j] < arr[min]) min = j;
    }
    if(min != i){
      swap(&arr[min], &arr[i]);
      render(arr, 10);
    }
  }
}

/** Best: n, Worst: n^2, Avg: n^2 */
void bubbleSort(int arr_len, int *arr)
{
  int n = arr_len;
  int newn, i;
  while(n > 0){
    newn = 0;
    for(i = 1; i<n; i++){
      if(arr[i-1] > arr[i]){
      	swap(&arr[i-1], &arr[i]);
      	newn = i;
      	render(arr, 1);
      }
    }
    n = newn;
  }
}

/** Best: n, Worst: n^2, Avg: n^2 */
void oddEvenSort(int arr_len, int *arr)
{
  int sorted = 0;
  while(! sorted){
    sorted = 1;
    for(int i=1; i < arr_len-1; i+=2){
      if(arr[i] > arr[i+1]){
        swap(&arr[i],&arr[i+1]);
        sorted = 0;
        render(arr,1);
      }
    }
    for(int i=0; i < arr_len-1; i+=2){
      if(arr[i] > arr[i+1]){
        swap(&arr[i],&arr[i+1]);
        sorted = 0;
        render(arr,1);
      }
    }
  }
}


/** Best: n, Worst: n^2, Avg: nlgn
 * Like a better version of bubbleSort.
 */
void combSort(int arr_len, int *arr)
{
  int gap = arr_len;
  float shrink_factor = 1.3f;
  int sorted = 0;
  while(! sorted){
    //gap = (int) ((float)gap)/shrink_factor;
    gap = floor(gap / shrink_factor);

    if(gap == 0){ gap = 1; sorted = 1; }
    else{ sorted = 0; }

    for(int i=0; arr_len > i+gap; i++){
      if(arr[i] > arr[i+gap]){
        swap(&arr[i], &arr[i+gap]);
        sorted = 0;
        render(arr, 1);
      }
    }
  }
}

/** Best: nlgn, Worst: n^2 depends on gap seq best known is nlg^2n, Avg: nlg^2n or n^(3/2) */
void shellSort(int arr_len, int *arr)
{
  int gaps[8] = {701,301,132,57,23,10,4,1}; //macin ciura's sequence
  for(int g=0; g<8; g++){
    int gap = gaps[g];
    for(int i= gap; i<arr_len; i++){
      int temp = arr[i];
      int j;
      for(j=i; j >= gap && arr[j - gap] > temp; j -= gap){
        arr[j] = arr[j - gap];
        render(arr,1);
      }
      arr[j] = temp;
      render(arr,1);
    }
  }
}

static void RECquickSort(int *arr, int low, int high)
{
  if(low < high){
    int pivot = arr[low];
    int i = low - 1;
    int j = high+ 1;
    while(1){
      while(arr[++i] < pivot);
      while(arr[--j] > pivot);
      if(i >= j) break;
      swap(&arr[i], &arr[j]);
      render(arr, 1);
    }
    RECquickSort(arr, low, j);
    RECquickSort(arr, j+1, high);
  }
}

/** Best: nlgn, Worst: n^2, Avg: nlgn */
void quickSort(int arr_len, int *arr)
{
  RECquickSort(arr, 0, arr_len-1);
}

#define min(a, b) (((a)<(b)) ? (a) : (b))

/** Best: nlgn, Worst: nlgn, Avg: nlgn */
void mergeSort(int arr_len, int *arr)
{
  int work_array[arr_len];
  for(int width=1; width < arr_len; width *= 2){
    for(int i=0; i < arr_len; i += 2*width){
      //bottom up merge
      int left = i;
      int right = min(i+width, arr_len);
      int end = min(i+2*width, arr_len);
      int p = left;
      int q = right;
      for(int k = left; k < end; k++){
        if(p < right && (q >= end || arr[p] <= arr[q])){
          work_array[k] = arr[p];
          p++;
        }else{
          work_array[k] = arr[q];
          q++;
        }
      }
    }

    //copyarray
    for(int k=0; k<arr_len; k++){
      arr[k] = work_array[k];
      render(arr, 1);
    }
  }
}

/* neuberts algorithm translated into C by Michael Sahota -- modifications by F. Fitzgerald */
static void RECflashSort(int arr[], int arr_len, int m_classes)
{
  int THRESHOLD = 75; /** if two adjacent values in vector are greater than this then we recurse???? idk why */
  int CLASS_SIZE = 75; /** min value of m */

  /** CLASS FORMATION */
  int nmin = 0, nmax = 0; //get min max indices
  for (int i = 0; i < arr_len; i++){
    if (arr[i] < arr[nmin]){ 
      nmin = i;
    }else if (arr[i] > arr[nmax]){ 
      nmax = i;
    }
  }

  // All the numbers are identical, the list is already sorted
  if (arr[nmax] == arr[nmin]) return;

  int min_val = arr[nmin];

  /** 
   * coefficient for expected index for a value ex. [1,4,6] (3-1)/(6-1) = 0.4
   * so that taking floor(val - min) == [(1-1)*0.4, (4-1)*0.4, (6-1)*0.4] = [0,1,2] 
   * ie the expected index for each val
  */
  float coeff_index = (m_classes - 1.0) / (arr[nmax] - min_val);

  /** allocate space for the l vector */
  int * l = calloc(m_classes, sizeof(int));
  l[0] = -1; /* since the base of the "a" (data) array is 0 */

  for (int i = 0; i < arr_len; i++){
    int exp_index = floor(coeff_index * (arr[i] - min_val)); //expected index
    l[exp_index] += 1; //population of class index
  }

  for (int k = 1; k < m_classes ; k++) l[k] += l[k-1];

  swap(&arr[nmax], &arr[0]); //sort largest element
  render(arr, 1);

  /**** PERMUTATION *****/
  /* moving elements into the correct class */
  int nmove = 0;
  int j = 0; //index of cycle leader
  int flash;
  int exp_index = m_classes - 1; //last index

  //scan through array
  while(nmove < arr_len){
    while(j > l[exp_index]){
      j++;
      exp_index = floor(coeff_index * (arr[j] - min_val));
    }

    flash = arr[j]; //cycle leader

    while(j <= l[exp_index]){
      exp_index = floor(coeff_index * (flash - min_val));
      swap(&arr[l[exp_index]], &flash);
      render(arr, 1);
      l[exp_index]--;
      nmove++;
    }
  }

  /**** Choice of RECURSION or STRAIGHT INSERTION *****/
  for (int k = 0; k<(m_classes - 1); k++){
    int nx;
    if ((nx = l[k+1] - l[k]) > THRESHOLD ){  /* then use recursion */
      RECflashSort(&arr[l[k]+1], nx, CLASS_SIZE);
    }else{  
      /* use insertion sort */
      for (int i = l[k+1]-1; i > l[k]; i--){
        if (arr[i] > arr[i+1]){
          int hold = arr[i];
          j = i;
          while(hold > arr[j+1]){
            arr[j] = arr[j+1];
            j++;
            render(arr, 1);
          }
          arr[j] = hold;
          render(arr, 1);
        }
      }
    }
  }
  free(l);   /* need to free the memory we grabbed for the l vector */
}

/** Best: n, Worst: n^2, Avg: n+r
 * sorts in n with O(1+m) memory when data is uniformly distributed by calculating expected index for each value
 * worst performance is when not uniformly distributed
 * sorts array by use of index vector l of dimension m, with m about 0.42 n // says 0.43 n on neuberts page
 * THRESHOLD is very important as it is the min number of elements required in subclass before recursion is used
*/
void flashSort(int arr_len, int *arr)
{
  RECflashSort(arr, arr_len, 0.42*arr_len);
}

/** Best: n^2, Worst: n^2, Avg: n^2
 * comparison sort theoretically optimal in terms of total number of writes
*/
void cycleSort(int arr_len, int *arr)
{
  int item, pos;
  for(int i=0; i < arr_len - 1; i++){
    item = arr[i];
    //find pos to insert item
    pos = i;
    for(int j=i+1; j < arr_len; j++)
      if(arr[j] < item) pos++;

    if(pos == i) continue; //item in right place already

    while(item == arr[pos]) pos++; //place item after any duplicate vals

    swap(&arr[pos], &item);
    render(arr, 1);

    //do the same for rest of cycle
    while(pos != i){
      pos = i;
      for(int j=i+1; j < arr_len; j++)
        if(arr[j] < item) pos++;

      while(item == arr[pos]) pos++;

      swap(&arr[pos], &item);
      render(arr, 1);
    }
  }
}

/** Best: n, Worst: n^2, Avg: n^2 */
//n+k, (where k is largest val in arr)
void countSort(int arr_len, int *arr)
{
  int k; //k will be the largest val in arr
  int mx = arr[0];
  for(int i=0; i<arr_len; i++){ if(arr[i] > mx) mx = arr[i]; }
  k = mx;
  int *cnt = calloc(sizeof(int), k+1); //since k has to be in the array the size is k+1

  //calc histograms of keys
  for(int i=0; i<arr_len; i++){ cnt[arr[i]] ++; }
  //calc start index for each key
  int total = 0, temp;
  for(int i =0; i <= k; i++){ temp = cnt[i]; cnt[i] = total; total += temp; }
  //finish
  int out[arr_len];
  for(int i=0; i<arr_len; i++){
    out[cnt[arr[i]]] = arr[i];
    cnt[arr[i]] ++;
    render(out, 1);
  }

  free(cnt);
}


/* amusing sorts */

/** Best: n, Worst: n^2, Avg: n^2 */
void gnomeSort(int arr_len, int *arr)
{
  for(int i=0; i<arr_len;){
    if(i == 0 || arr[i] >= arr[i-1]){
      i++;
    }else{
      swap(&arr[i], &arr[i-1]);
      i--;
      render(arr, 1);
    }
  }
}

static void RECstoogeSort(int *arr, int i, int j)
{
  if(arr[i] > arr[j]){
    swap(&arr[i], &arr[j]);
    render(arr, 1);
  }
  if((j-i+1) > 2){
    int t = (j-i+1)/3;
    RECstoogeSort(arr, i, j-t);
    RECstoogeSort(arr, i+t, j);
    RECstoogeSort(arr, i, j-t);
  }
}

/** Best: n^2.7, Worst: n^2.7, Avg: n^2.7 */
void stoogeSort(int arr_len, int *arr)
{
  RECstoogeSort(arr, 0, arr_len-1);
}

/** Best: n, Worst: n*n!, Avg: n*n! ?? */
void monkeySort(int arr_len, int *arr)
{
  int sorted = 0;
  while(! sorted){
    sorted = 1;
    for(int i=0; i<arr_len-1; i++){
      if(arr[i] > arr[i+1]){
        int p = rand() % arr_len;
        int q = rand() % arr_len;
        swap(&arr[p], &arr[q]);
        render(arr, 1);
        sorted = 0;
        break;
      }
    }
  }
}
