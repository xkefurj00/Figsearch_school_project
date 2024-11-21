#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILEPATH "obrazek.txt"

struct Image
{
   int number_of_lines;  // variable that stores number of lines
   int number_of_columns; // variable that stores number of columns
   int **colors; // field of bitmap colors
};


int bitmap_ctor(struct Image *image){

image->colors = (int**)malloc(image->number_of_lines*sizeof(int*));
// Allocation of memory for rows

if (image->colors==NULL){

    fprintf(stderr,"Memory allocation failed!");
    return 1;

}// Checks if malloc was successfull

for (int row = 0; row < image->number_of_lines; row++)
{
    image->colors[row] = (int*)malloc(image->number_of_columns*sizeof(int));

    if (image->colors[row]==NULL){

    fprintf(stderr,"Memory allocation failed!");
    return 1;

    }// Checks if malloc was successfull

}// Allocation of memory for columns

return 0;

}



void bitmap_dtor(struct Image *image){

for (int row = 0; row < image->number_of_lines; row++)
{
    free(image->colors[row]);
}// Free memory for columns


    free(image->colors); 
    // Free memory for rows
    image->colors=NULL;
    image->number_of_lines=0;
    image->number_of_columns=0;

}

int load_file(struct Image *image, FILE *bitmap){



fscanf(bitmap,"%d %d", &image->number_of_lines, &image->number_of_columns);
// Loads first two numbers which are size of image

if (bitmap_ctor(image) !=0){
    return 1;
}


for (int row = 0; row < image->number_of_lines; row++){

    for (int column = 0; column < image->number_of_columns; column++)
    {
        fscanf(bitmap,"%d", &image->colors[row][column]);
    }
    
}


return 0;
}



void print_help(){




}

void find_hline(struct Image *image){

int longest1 = 0;
int longest2 = 0;
int row_number;
int column_number_start;
int column_number_end;


for (int row = 0; row < image->number_of_lines; row++)
{
    for (int column = 0; column < image->number_of_columns; column++)
    {
        if (image->colors[row][column]==1)
        {
            longest1++;
        }

        if ((image->colors[row][column]==0)  || (column == image->number_of_columns - 1))
        {
                if (longest1 > longest2)
                {
                    longest2 = longest1;
                    row_number = row;
                    if (column == longest2){
                    column_number_start = column - longest2;
                    column_number_end = column - 1;
                    }
                    else{
                    column_number_start = column - longest2 +1;
                    column_number_end = column;
                    }
                }
            longest1 = 0;
        } 
        
        

    }

}

fprintf(stdout,"%d %d %d %d", row_number, column_number_start ,
row_number, column_number_end);

}

void find_vline(struct Image *image){
    
int longest1 = 0;
int longest2 = 0;
int column_number;
int row_number_start;
int row_number_end;


for (int column = 0; column < image->number_of_columns; column++)
{
    for (int row = 0; row < image->number_of_lines; row++)
    {
        if (image->colors[row][column]==1)
        {
            longest1++;
        }

        if ((image->colors[row][column]==0)  || (row == image->number_of_lines - 1))
        {
                if (longest1 > longest2)
                {
                    longest2 = longest1;
                    column_number = column;
                    if (row == longest2)
                    {
                        row_number_start = row - longest2;
                        row_number_end = row - 1;
                    }else{
                    row_number_start = row - longest2 + 1;
                    row_number_end = row;
                    }
                    
                    
                    
                }
            longest1 = 0;
        } 
        
        

    }

}

fprintf(stdout,"%d %d %d %d",  row_number_start , column_number, row_number_end, column_number);


}

void find_square(struct Image *image){

int row_number_start, row_number_end, column_number_start, column_number_end;
int square_size = 0;
int largest_square_size = 0;
int counter;
int valid;

for (int row = 0; row < image->number_of_lines; row++)
{
    for (int column = 0; column < image->number_of_columns; column++)
    {
       

        if (image->colors[row][column] == 1)
        {
            valid = 1 ;

            for (square_size = 0; (square_size + column < image->number_of_columns) 
                && (square_size + row < image->number_of_lines); square_size++)
            {  
                

                for (counter = 0; counter <= square_size && (row + counter < image->number_of_lines) &&
                (column + counter < image->number_of_columns) ; counter++)
                {   

                if (image->colors[row + square_size][column] == 1 &&
                        image->colors[row][column + square_size] == 1)
                    {
                        if ((image->colors[row + square_size][column + counter] == 0) 
                        || (image->colors[row + counter][column + square_size] == 0))
                        {   
                        
                        valid = 0;
                        break;
                    
                        }

                    } else valid = 0;
        
                }

             //   fprintf(stdout, "R:%d, CL:%d, S:%d, C:%d V:%d\n" , row, column, square_size, counter, valid);
                 
                if ((square_size >largest_square_size)){

                    largest_square_size = square_size;
                    row_number_start = row;
                    column_number_start = column;    
                    row_number_end = row + largest_square_size-1;
                    column_number_end =  column + largest_square_size-1;
                }
               if(valid ==0) break;

        }
        

        }
    }
    
}

if (largest_square_size > 0)
{
    fprintf(stdout, "%d %d %d %d\n", row_number_start, column_number_start,
    row_number_end, column_number_end);
} else fprintf(stdout, "No square found.");


}

void use_function(struct Image *image, char *function){

if (strcmp(function, "hline")==0)
{
   find_hline(image);
}
if (strcmp(function, "vline")==0)
{
   find_vline(image);
}
if (strcmp(function, "square")==0)
{
   find_square(image);
}

}



int main(int argc, char *argv[]){

struct Image image;


FILE *bitmap=fopen(argv[2], "r");
if (bitmap == NULL)
{
    fprintf(stderr, "Error: Cant open file");
    return 1;
}





if (load_file(&image, bitmap) != 0)
{
    fclose(bitmap);
    return 1;
}

if (argc==3)
{
    use_function(&image , argv[1]);    
}

fclose(bitmap);

bitmap_dtor(&image);


return 0;

}