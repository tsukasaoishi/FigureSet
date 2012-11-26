//************************************
//  figure_set.h
//
//  Tsukasa OISHI
//  http://www.kaeruspoon.net
//************************************

#include <math.h>

#ifndef RUBY_19
#ifndef RFLOAT_VALUE
#define RFLOAT_VALUE(v) (RFLOAT(v)->value)
#endif
#ifndef RARRAY_LEN
#define RARRAY_LEN(v) (RARRAY(v)->len)
#endif
#ifndef RARRAY_PTR
#define RARRAY_PTR(v) (RARRAY(v)->ptr)
#endif
#endif

// max children size of branch
#define MAX_CHILDREN_SIZE_OF_BRANCH (unsigned long)16
#define LAST_BRANCH_LEVEL (unsigned long)6

#if ULLONG_MAX == 0xffffffffffffffff
#define ULONG_BIT_COUNT (unsigned long)64
#define MAX_CHILDREN_SIZE_OF_ROOT_NODE (unsigned long)4
#define BIT_COUNT(x) bit64_count(x)
static unsigned long OFFSET_SCALE[] = {0x40000000, 0x4000000, 0x400000, 0x40000, 0x4000, 0x400, 0x40};
#else
#define ULONG_BIT_COUNT (unsigned long)32
#define MAX_CHILDREN_SIZE_OF_ROOT_NODE (unsigned long)8
#define BIT_COUNT(x) bit32_count(x)
static unsigned long OFFSET_SCALE[] = {0x20000000, 0x2000000, 0x200000, 0x20000, 0x2000, 0x200, 0x20};
#endif

// branch node children type
#define CT_BRANCH 0
#define CT_LEAF 1

// valid valie range
#define VALID_MIN_VALUE UINT2NUM(0)
#define VALID_MAX_VALUE UINT2NUM(0xffffffff)

// FigureSet class object
static VALUE rb_cFigureSet;

// leaf node
// example:
//   data = 5  # (bit is 0...0101) exist bit numbers is (2, 0) 
//   offset = 640
// 
// set is
//  [offset + 2, offset + 0]
//   = [642, 640]
//  
typedef struct _leaf_node {
    unsigned long offset;   // offset number
    unsigned long data;  // each bit is exist flag(1:exist 0:none) of member
} *leaf_node;

// branch node
typedef struct _branch_node {
    unsigned char children_type; // children type
    unsigned char children_size;
    void *index[MAX_CHILDREN_SIZE_OF_BRANCH];  // children pointer
} *branch_node;

typedef struct _root_node {
    unsigned char children_size;     
    unsigned long size;              // number of elements in set
    void *index[MAX_CHILDREN_SIZE_OF_ROOT_NODE];    // children pointer
} *root_node;

// initialize
void init_root_node(root_node);
void *init_branch_node();
void *init_leaf_node(unsigned long);

// init_copy
void copy_root_node(root_node, root_node);
void copy_branch_node(root_node, branch_node, branch_node);
void *init_and_copy_brance_node(root_node, branch_node);
void *init_and_copy_leaf_node(root_node, leaf_node);

// insert element into set
void add_num(root_node, unsigned long);
unsigned int search_and_insert(branch_node, unsigned long, unsigned long, unsigned long);
unsigned int search_and_insert_at_leaf(leaf_node, unsigned long);

// remove element from set
void delete_num(root_node, unsigned long);
unsigned int search_and_remove(branch_node, unsigned long, unsigned long, unsigned long);
unsigned int search_and_remove_at_leaf(leaf_node, unsigned long);

// output Array object from internal set
void to_array(root_node, VALUE);
void search_and_get_array(branch_node, VALUE);
void search_and_get_array_at_leaf(leaf_node, VALUE);

// join
void join(root_node, root_node, root_node);
void middle_join_branch_node(root_node, branch_node, branch_node, branch_node);
void last_join_branch_node(root_node, branch_node, branch_node, branch_node);
void *init_and_join_brance_node(root_node, branch_node, branch_node);
void *init_and_join_leaf_node(root_node, leaf_node, leaf_node);

// intersection
void intersection(root_node, root_node, root_node);
void middel_intersection_branch_node(root_node, branch_node, branch_node, branch_node);
void last_intersection_branch_node(root_node, branch_node, branch_node, branch_node);
void *init_and_intersection_leaf_node(root_node, leaf_node, leaf_node);
void *init_and_intersection_branch_node(root_node, branch_node, branch_node);
unsigned long bit32_count(unsigned long);
unsigned long bit64_count(unsigned long);

// sample
void sample(root_node, VALUE, unsigned long);
void search_and_sample_array(branch_node, VALUE);
void search_and_sample_array_at_leaf(leaf_node, VALUE);

// memory free
void destroy_all(root_node);
void destroy_all_branches(root_node);
void destroy_branch(branch_node);

//-----------------------------------------------------------
// Ruby Methods
// ----------------------------------------------------------

static VALUE t_allocate(VALUE);
static VALUE t_initialize(int, VALUE *, VALUE);
static VALUE t_initialize_copy(VALUE, VALUE);
static VALUE t_add(VALUE, VALUE);
static VALUE t_delete(VALUE, VALUE);
static VALUE t_intersection(VALUE, VALUE);
static VALUE t_union(VALUE, VALUE);
static VALUE t_to_a(VALUE);
static VALUE t_sample(int, VALUE *, VALUE);
static VALUE t_size(VALUE);
static VALUE t_empty(VALUE);
static VALUE t_clear(VALUE);
void Init_figure_set(void);
