/*
* FILE: id3.c
*
* Author: Andrew Colin
* Modifier: Fang Xiaolin    E-mail: swimmingfox126.com
*
* DISCLAIMER: No liability is assumed by the author for any use made
* of this program.
*
* DISTRIBUTION: Any use may be made of this program, as long as the
* clear acknowledgment is made to the author in code and runtime
* executables
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#include "id3.h"
#include "proto.h"

/*-------------------------------------------------------------------*/

MATRIX *matrix;  /* ������������ */ 

CHAR **tag_names;  /* ���������� */ 

UINT target,       /* ����������һ��,��type������һ�� */ 
     n_vars,       /* �����м��� */ 
     n_samples;    /* �����м��� */ 

UINT *n_dif_vars;  /* �����м�����ͬ��ֵ */ 
STRING **dif_vars; /* ���в�ͬ��ֵ */ 

UINT MAX_N_DIF_VARS = 10; /* ����ÿ�����Ĳ�ֵͬ����Ϊ10 */ 

UINT *used;        /* �ڽ���ʱ�����ж���Щ�б��������� */ 


int main (int argv, char *argc[])
{
    freopen("output.txt", "w", stdout);
    
    NODE *node;
    
    CHAR data_file[13], tag_file[13];  /* Longest file name in DOS */
    
  
    UINT i;

    strcpy(data_file, "input_1.txt");
    strcpy(tag_file,  "input_2.txt");
    
    /* �������ж����ж�����,�����浽n_samples��n_vars */
    file_size (data_file);
    
    /* ����ռ��n_dir_vars, dir_vars, �Լ�used*/
    n_dif_vars = Allocate_n_dif_vars ( );
    dif_vars = Allocate_dif_vars ();
    used = Allocate_used ( );
    
    /* �Ҹ��в�ͬ��ֵ,����¼��dif_vars[][]�� */ 
    find_dif_vars (data_file);    

    /* ���������� */
    tag_names = read_tags (tag_file);

    /* ����ռ��matrix */
    matrix = build_matrix ();

    /* ������������ */
    read_matrix (data_file);

    /* ����Ŀ�������һ�� */
    target = n_vars - 1;
    
    /* ��ʼ�����ж���û�������� */ 
    for (i=0; i<n_vars; i++)
        used[i] = 0;
    
    /* ���ؾ��������ڵ��node */ 
    node = ID3 ( NULL, 0, used );
    
    /* ��������� */ 
    print_tree(node, 0);
    
    /* �ͷſռ� */ 
    free_tags ( );
    free_matrix ( );
    free_tree (node);

    return 0;
}

/*-------------------------------------------------------------------*/

void print_type ( NODE *node )
{
    STRING **data;
    NODE *_local;
    UINT i;
    BOOL _match;
    
    data = matrix->data;
    
    for (i=0; i<n_samples; i++) 
    {
        _match = true;
        _local = node;
        while (_local->parent != NULL) 
        { 
            /* If at the root node, all entries match*/

            if ( strcmp(data[i][_local->parent->tag_idx], dif_vars[_local->parent->tag_idx][_local->dif_var_idx]) != 0 ) {
                _match = false;
                break;
            }    
          
            _local = _local->parent;
        }
        
        if (_match == true)
        {
            printf("[%8s: %-4s]\n", "type", data[i][target]);
            break;
        }    
    }    
}    

/*-------------------------------------------------------------------*/

void free_tree ( NODE *node ) {
    UINT i;
    
    if (node == NULL)
        return;
    if (node->child == NULL)
    {   
        free ( node );
        return;
    }   
    
    for (i=0; i<n_dif_vars[node->tag_idx]; i++)
    {
        free ( node->child[i] );
    }
    
    free ( node );
}    

/*-------------------------------------------------------------------*/

void print_tree ( NODE *node , UINT level )
{
    UINT i, j, k;
    
    if (node == NULL) 
    {
        printf("%d  000000000000\n", level);
        return;
    }    
    if (node->child == NULL)
    {
        if (node->parent == NULL)
            printf("[root]          \n");
        else 
            printf("[%8s: %-4s]", tag_names[node->parent->tag_idx], dif_vars[node->parent->tag_idx][node->dif_var_idx]);
        print_type ( node );
        return;
    } 
    else 
    {   
        if (node->parent == NULL)
            printf("[root]          \n");
        else 
            printf("[%8s: %-4s]", tag_names[node->parent->tag_idx], dif_vars[node->parent->tag_idx][node->dif_var_idx]);
    
        for (i=0; i<n_dif_vars[node->tag_idx]; i++)
        {
            if(i>0)
                for(j=0; j<level; j++)
                    for(k=0; k<16; k++)
                        printf(" ");
            print_tree ( node->child[i], level+1);
        }    
    }    
}
   
/*-------------------------------------------------------------------*/

NODE* ID3 ( NODE* parent, UINT state, UINT *used )

/* Routine to build a decision tree, based on Quinlan's ID3 algorithm. */
{
    NEGENTROPY negentropy_struct;
    NODE *node, *_parent;
    UINT n_vars = matrix->width, n_samples = matrix->height, i, j, split;
    STRING **data = matrix->data;
    REAL max_negentropy, _negentropy;
    UINT status;
    
    /* Allocate memory for this node */
    node = (NODE*) malloc (sizeof(NODE));
    if (!node)
        err_exit (__FILE__, __LINE__);
    
    node->parent = parent; /* Set address of parent node */
    
    if (parent == NULL)
        node->dif_var_idx = INVALID_DIF_VAR_IDX;
    else 
        node->dif_var_idx = state;
  
    /*
     * Select attribute with lowest negentropy for splitting. Scan through
     * ALL attributes (except the target) and ALL data samples. This is
     * pretty inefficient for data sets with repeated values, but will do
     * for illustrative purposes
     */
    
    _parent = parent;
    
    status = INACTIVE;
    max_negentropy = 0.0;
    split = 0;

    for (i=0; i<n_vars; i++) /* ��ÿһ�ж��������� */ 
    {
        
        if (i != target && !used[i]) /* ��i�в��Ǿ���Ŀ����һ��,��δ�������� */ 
        {
            
            negentropy_struct = negentropy (i, node); /* �����i�е���ֵ */ 

            _negentropy = negentropy_struct.ne; /* ��ʱ������ֵ�� _negentropy */

            
            if (_negentropy > max_negentropy) /* ��max_negentropy�Ƚ�,����������ֵ,���������ֵ����һ�м�¼��split,״̬��¼��status*/
            {
                max_negentropy = _negentropy;
                split = i;
                status = negentropy_struct.status;
            }
                                
        } /* if (i != target && !used[i]) */

    } /* for (i=0; i<n_vars; i++) */
    


    if (status == INACTIVE) /* ��Ҷ�ӽڵ� */ 
    {
        node->tag_idx = INVALID_TAG_IDX; /* ����Ҫ�ټ����������� */ 
        node->child = NULL;
        return node;
    }    
    
    else { /* ����ΪҶ�ӽڵ� */
     
        node->tag_idx = split; /* ��split�м��������� */ 

        node->child = (NODE**) malloc (n_dif_vars[split] * sizeof(NODE*)); /* split��һ�����ֲ�ͬ��ֵ,���ж��ٸ����� */ 
        if (!node->child)
            err_exit (__FILE__, __LINE__);
        
        for (i=0; i<n_dif_vars[split]; i++) /* �����������ӽڵ� */ 
        {
            used[split] = 1; /* split�����Ѿ���������,��Ϊ1 */ 
            node->child[i] = ID3 ( node, i, used );
            used[split] = 0; /* ��������������Ϊ0 */ 
        }    

        return node;
    }    
}
 

/*-------------------------------------------------------------------*/

NEGENTROPY negentropy (
    UINT   column,
    NODE   *node)
{
    /*
     * Calculates the entropy of classification of an attribute, given
     * a table of attributes already used, the attribute on which splitting
     * is to be taken, and the target attribute. Entropy is calculated in
     * bits, so logs are taken to base 2 by dividing by LN_2.
     *
     * The returned value always lies in the (closed) range [0, 1].
     */

    NEGENTROPY ret_val;
    NODE *_local;
    UINT i, j, _match;
    REAL MES, M, B, p;
    STRING **data = matrix->data;
    
    
    UINT **count;
    UINT *tem_count;
    UINT *type_count;
    UINT total;
    UINT tem_row, tem_col;
    
    /* Allocate memory for this count */
    count = (UINT**) malloc (n_dif_vars[column] * sizeof(UINT*));
    if (!count)
        err_exit (__FILE__, __LINE__);
    
    for (i=0; i<n_dif_vars[column]; i++)
    {
        count[i] = (UINT*) malloc (n_dif_vars[target] * sizeof(UINT));
        if (!count[i])
            err_exit (__FILE__, __LINE__);
    }
    
    tem_count = (UINT*) malloc (n_dif_vars[column] * sizeof(UINT));
    if (!tem_count)
        err_exit (__FILE__, __LINE__);
        
    type_count = (UINT*) malloc (n_dif_vars[target] * sizeof(UINT));
    if (!type_count)
        err_exit (__FILE__, __LINE__);
    
    /* Initial count */
    for (i=0; i<n_dif_vars[column]; i++)
        for (j=0; j<n_dif_vars[target]; j++)
            count[i][j] = 0;
    
    for (i=0; i<n_dif_vars[column]; i++)
        tem_count[i] = 0;
        
    for (i=0; i<n_dif_vars[target]; i++)
        type_count[i] = 0;
    
    total = 0;

    /* ��ÿһ�����ݶ�����ɨ�� */ 
    for (i=0; i<n_samples; i++) 
    {

        _match = 1;
        _local = node;
        while (_local->parent != NULL) 
        { 
            /* If at the root node, all entries match*/

            if ( strcmp(data[i][_local->parent->tag_idx], dif_vars[_local->parent->tag_idx][_local->dif_var_idx]) != 0 ) {
                _match = 0;
                break;
            }    
          
            _local = _local->parent;
        }

        if (_match) 
        {
            for (tem_row=0; tem_row<n_dif_vars[column]; tem_row++)
            {
                if ( strcmp( data[i][column], dif_vars[column][tem_row] ) == 0 )
                    break;
            }
            
            for (tem_col=0; tem_col<n_dif_vars[target]; tem_col++)
            {
                if ( strcmp( data[i][target], dif_vars[target][tem_col] ) == 0 )
                    break;
            } 
            
            /* һЩ����, Ϊ��������ָ����ĸ���p�� */ 
            count[tem_row][tem_col]++;
            tem_count[tem_row]++;
            type_count[tem_col]++;
            total++;
            
        }
    }   /* for (i=0; i<n_samples; i++) */
    
    for (i=0; i<n_dif_vars[target]; i++)
    {
        p = (REAL) type_count[i] / (REAL) total;
        MES += -entropy (p);
    }
 

    if (total == 0) /* �����û���ҵ�������,����ΪҶ�ӽڵ� */ 
    {
        ret_val.ne = 1.0;
        ret_val.status = INACTIVE;
        return ret_val;
    } 
    
    for (i=0; i<n_dif_vars[target]; i++)
        if (type_count[i] == total)
            break;
    if (i < n_dif_vars[target]) /* ȫ����һ��Ͳ����ٷ���,Ҳ��ΪҶ�ӽڵ� */ 
    {
        ret_val.ne = 1.0;
        ret_val.status = INACTIVE;
        return ret_val;
    }
    
    /* ��������ü����صĹ�ʽ������ */
     
    M = 0.0;
    B = 0.0;
    for (i=0; i<n_dif_vars[column]; i++)
    {
        if (tem_count[i] == 0) continue;
        
        M = 0.0;
        for (j=0; j<n_dif_vars[target]; j++)
        {
            p  = (REAL) count[i][j] / (REAL) tem_count[i];
            M += -entropy (p);
        }   
        p = (REAL) tem_count[i] / (REAL) total;
        B += p * M; 
    }
    
    MES = 0.0;
    for (i=0; i<n_dif_vars[target]; i++)
    {
        p = (REAL) type_count[i] / (REAL) total;
        MES += -entropy (p);
    }    
    
    ret_val.ne = MES - B;
    ret_val.status = ACTIVE;

    return ret_val;
}

/*-------------------------------------------------------------------*/

void read_matrix (CHAR *filename)

{

    UINT i, j;
    UINT height = matrix->height;
    UINT width  = matrix->width;
    STRING **data = matrix->data;
    FILE *f;

    /* Open price file */
    if ((f = fopen(filename, "r")) == NULL) 
    {
        printf("\n File not found : %s\n", filename);
        err_exit (__FILE__, __LINE__);
    }

    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            fscanf(f, "%s", &data[i][j] );
            printf("%-15s", data[i][j]);
        }
        printf("\n");
    }    

    fclose(f);

}

/*-------------------------------------------------------------------*/

MATRIX *build_matrix ( )
{
    MATRIX *_matrix;
    UINT i;

    _matrix = (MATRIX*) malloc (sizeof (MATRIX));
    if (!_matrix)
        err_exit (__FILE__, __LINE__);

    _matrix->width  = n_vars;
    _matrix->height = n_samples;

    _matrix->data = (STRING**) malloc (n_samples * sizeof (STRING*));
    if (_matrix->data == NULL)
        err_exit(__FILE__, __LINE__);

    for (i=0; i<n_samples; i++)
    {
        _matrix->data[i] = (STRING*) malloc (n_vars * sizeof(STRING));
        if (_matrix->data[i] == NULL)
            err_exit(__FILE__, __LINE__);
    }
    return _matrix;
}

/*-------------------------------------------------------------------*/

/*
* Standard error handler function
*/

void err_exit (CHAR* file, UINT line)
{
    printf("\n Fatal error in file %s, line %u", file, line);
    exit(0);
}

/*-------------------------------------------------------------------*/

void file_size (CHAR *file_name)
/*
* Given the name of a file of numeric data, this routine counts
* the numbers of rows and columns. It's assumed that the number
* of entries is the same in each row, and an error is flagged if this
* is not the case.
*
*/
{
    FILE *f;
    UINT buf_size = 0xFF, _width = 0;
    CHAR *buffer, *ptr;
    CHAR *delimiters = " ,\t\n";

    n_vars = n_samples = 0;

    buffer = (CHAR*) malloc (buf_size * sizeof (CHAR));
    if (buffer == NULL)
        err_exit (__FILE__, __LINE__);

    /* Open price file - abort if filename invalid */
    f = fopen(file_name, "r");
    if (f == NULL)
    {
        printf("\n File not found : %s\n", file_name);
        err_exit (__FILE__, __LINE__);
    }

    /* Get number of entries in first row */
    if (fgets(buffer, buf_size, f) != NULL)
    {
        ++n_samples;
        ptr = strtok (buffer, delimiters);
        while (ptr != NULL)
        {
            ++n_vars;
            ptr = strtok (NULL, delimiters);
        }
    }

    /* Count numbers of subsequent rows */
    while (!feof(f))
    {
        if (fgets(buffer, buf_size, f) != NULL)
        {
            if (strlen(buffer) > strlen("\n"))  /* if line is more than a NL char */
            {
                ++n_samples;
                _width = 0;
                ptr = strtok (buffer, delimiters);
                while (ptr != NULL)
                {
                    ++_width;
                    ptr = strtok (NULL, delimiters);
                }

                if (n_vars != _width)
                {
                    printf("\n Number of entries in file %s did not agree", file_name);
                    err_exit (__FILE__, __LINE__);
                }
            }
        }
    }
    
    free (buffer);
}

/*-------------------------------------------------------------------*/

void free_matrix ( )
{
    UINT i;
    for (i=0; i<matrix->height; i++)
        free (matrix->data[i]);

    free (matrix->data);
    free (matrix);
}

/*-------------------------------------------------------------------*/

void free_tags ( )
{
    UINT i;
    for (i=0; i<n_vars; i++)
        free(tag_names[i]);
    free (tag_names);
}

/*-------------------------------------------------------------------*/

CHAR **read_tags (CHAR *tag_file)
{
    FILE *f;
    CHAR **tem_tag_names;
    UINT i;
    CHAR buffer[0xFF];
    CHAR *ptr;
    CHAR *delimiters = " ,\t\n";

    f = fopen(tag_file, "r");
    if (f == NULL)
    {
        printf("\n File not found : %s\n", tag_file);
        err_exit (__FILE__, __LINE__);
    }

    tem_tag_names = (CHAR**) malloc (n_vars * sizeof (CHAR*));
    for (i=0; i<n_vars; i++)
        tem_tag_names[i] = (CHAR*) malloc (0xFF * sizeof (CHAR));

    i = 0;
    while (!feof(f))
    {    
        if (fgets(buffer, 0xFF, f) != NULL)
        {
            if (strlen(buffer) > strlen("\n"))
            {
                if (i > n_vars-1)
                {
                    printf("\nMore variable names were detected than data items.");
                    printf("\nPlease correct this problem before proceeding");
                    exit(0);
                }
                //sscanf (buffer, "%[a-zA-Z0-9-_;:!@#$%^&*(){}[]]", tem_tag_names[i]);
                //i++;

                ptr = strtok (buffer, delimiters);
                while (ptr != NULL)
                {   
                    printf("%-15s", ptr);
                    strcpy(tem_tag_names[i], ptr);
                    i++;
                    ptr = strtok (NULL, delimiters);
                }
                
            }
        }
        
    }

    if (i<n_vars) 
    {
        printf("\nFewer variable names than data items were detected.");
        printf("\nPlease correct this problem before proceeding");
        exit(0);
    }
    printf("\n\n");

    fclose (f);

    return tem_tag_names;
    
}

/*-------------------------------------------------------------------*/

void find_dif_vars (CHAR *file_name)
{
    FILE *f;
    UINT buf_size = 0xFF, _width = 0;
    CHAR *buffer, *ptr;
    CHAR *delimiters = " ,\t\n";
    UINT i = 0;

    buffer = (CHAR*) malloc (buf_size * sizeof (CHAR));
    if (buffer == NULL)
        err_exit (__FILE__, __LINE__);

    /* Open price file - abort if filename invalid */
    f = fopen(file_name, "r");
    if (f == NULL)
    {
        printf("\n File not found : %s\n", file_name);
        err_exit (__FILE__, __LINE__);
    }
    
    /* Get number of entries in first row */
    if (fgets(buffer, buf_size, f) != NULL)
    {
        i = 0;
        ptr = strtok (buffer, delimiters);
        while (ptr != NULL)
        {
            n_dif_vars[i] = 0;
            if( sizeof(dif_vars[i][n_dif_vars[i]]) < strlen(ptr) )
                err_exit(__FILE__, __LINE__);

            strcpy(dif_vars[i][n_dif_vars[i]], ptr);
            n_dif_vars[i] = 1;
            i++;
            
            ptr = strtok (NULL, delimiters);
        }
    }

    /* Count numbers of subsequent rows */
    while (!feof(f))
    {
        if (fgets(buffer, buf_size, f) != NULL)
        {
            if (strlen(buffer) > strlen("\n"))  /* if line is more than a NL char */
            {
                i = 0;
                ptr = strtok (buffer, delimiters);
                while (ptr != NULL)
                {
                    if( sizeof(dif_vars[i][n_dif_vars[i]]) < strlen(ptr) )
                        err_exit(__FILE__, __LINE__);

                    if( exsit(i, n_dif_vars[i], ptr) == false )
                    {
                        strcpy(dif_vars[i][n_dif_vars[i]], ptr);
                        n_dif_vars[i]++;
                    }  
                    i++;
                    
                    ptr = strtok (NULL, delimiters);
                }
            }
        }
    }

    free (buffer);

}   

/*-------------------------------------------------------------------*/

UINT *Allocate_n_dif_vars () {
    UINT *tem_n_dif_vars;
    tem_n_dif_vars = (UINT*) malloc (n_vars * sizeof (UINT));
    if (tem_n_dif_vars == NULL)
        err_exit(__FILE__, __LINE__);
    return tem_n_dif_vars;
}  

/*-------------------------------------------------------------------*/

UINT *Allocate_used () {
    UINT *tem_used;
    tem_used = (UINT*) malloc (n_vars * sizeof (UINT));
    if (tem_used == NULL)
        err_exit(__FILE__, __LINE__);
    return tem_used;
}  

/*-------------------------------------------------------------------*/

STRING **Allocate_dif_vars () {
    STRING **tem_dif_vars;
    UINT i;
    
    tem_dif_vars = (STRING**) malloc (n_vars * sizeof (STRING*));
    
    if (tem_dif_vars == NULL)
        err_exit(__FILE__, __LINE__);
        
    for (i=0; i<n_vars; i++)
    {
        tem_dif_vars[i] = (STRING*) malloc (MAX_N_DIF_VARS * sizeof(STRING));
        if (tem_dif_vars[i] == NULL)
            err_exit(__FILE__, __LINE__);
    }
    return tem_dif_vars;
} 
 
/*-------------------------------------------------------------------*/

bool exsit (UINT m, UINT n, CHAR *ptr)
{
    UINT j;
    for(j=0; j<n; j++)
    {
        if( strcmp(dif_vars[m][j], ptr) == 0 )
            return true;
    }
    return false;
}        
    
