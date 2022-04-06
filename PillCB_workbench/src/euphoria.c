#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>

main() {
    int size = 0;
    FILE* input = fopen("[MP3DOWNLOAD.TO]LabrinthZendaya-AllForUs(Lyrics)-320k.mp3.midi", "r");                 // file stream to read file
    char* loadedArray;                                  // initializes long array to store long ints from file
    if(input == NULL)                                   // checks if file was opened incorrectly
    {
        size = 0;                                      // sets size to 0 as given
        loadedArray = NULL;                             // points the array to NULL as given
    }
    else
    {
        fseek(input,0,SEEK_END);                        // first seeks to end of file
        size = (ftell(input) / (sizeof(char)));        // calculates the number of long ints in the file by dividing the cursor position at the end of the file by the size of a long
        fseek(input,0,SEEK_SET);                        // back to start
        loadedArray = calloc(size,sizeof(char));       // sets array to appropriate size but initially all elements are 0
        fread(loadedArray, sizeof(char), size, input); // actually transcribes the file's long ints into the array
    }
    fclose(input);

    FILE* output = fopen("output", "w+");   // file stream to write/create file
    if(output == NULL)                      // checks to see if file opened correctly
    {
        return 0;                           // if incorrect, returns number of long ints stored which would be 0
    }
    for(int i = 0; i < size; i++)
    {
        fwrite("0x",2*sizeof(char),0,output); // transcribes array into output file
        fwrite(loadedArray[i],sizeof(char),0,output); // transcribes array into output file
        fwrite(",",2*sizeof(char),0,output); // transcribes array into output file
    }
    fclose(output);
    return size;
}
