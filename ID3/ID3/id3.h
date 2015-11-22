typedef unsigned int  UINT;
typedef unsigned long ULONG;
typedef          char CHAR;
typedef unsigned char BOOL;
typedef double        REAL;
typedef          char STRING[20];

typedef struct node {
   UINT tag_idx; /* ID code for attribute */
   UINT dif_var_idx;
   struct node *parent; /* Addess of parent node */
   struct node **child; /* Address of 'on' node */
} NODE;

typedef struct ne_struct {
    REAL ne;
    UINT status;
} NEGENTROPY;

typedef struct matrix {
   UINT width;
   UINT height;
   STRING **data;
} MATRIX;

enum { 
    INACTIVE = 0, 
    ACTIVE   = 1,
    INVALID_TAG_IDX = 0XFFFF,
    INVALID_DIF_VAR_IDX = 0XFFFF
};
#define LN_2 0.693147180559945309417
#define entropy(x) (x > 0 ? x * log(x) / LN_2 : 0.0)

