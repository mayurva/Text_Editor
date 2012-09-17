#include <stdio.h>
#include <stdlib.h>

enum back_ptr_t {null,left,right};

typedef int key_t;
typedef int height_t;
typedef struct tr_n_t {
	key_t key; 
	struct tr_n_t *left;
	struct tr_n_t *right;
	enum back_ptr_t back_ptr;
	int height;
} tree_node_t;
typedef tree_node_t text_t;
typedef char object_t;

#define BLOCKSIZE 256

tree_node_t *currentblock = NULL;
int size_left;
tree_node_t *free_list = NULL;
int nodes_taken = 0;
int nodes_returned = 0;

int get_max(int a,int b) {
	return a>b?a:b;
}

tree_node_t *get_node()
{
	tree_node_t *tmp;
	nodes_taken += 1;
	if( free_list != NULL ) {
		tmp = free_list;
		free_list = free_list -> right;
	}
	else {
		if( currentblock == NULL || size_left == 0) {
			currentblock = (tree_node_t *) malloc( BLOCKSIZE * sizeof(tree_node_t) );
			size_left = BLOCKSIZE;
		}
		tmp = currentblock++;
		size_left -= 1;
	}
	tmp->back_ptr = null;
	return(tmp);
}

void return_node(tree_node_t *node)
{
	node->right = free_list;
	free_list = node;
	nodes_returned +=1;
}

/*tree_node_t *create_tree(void)
{
}*/

/*object_t *find_iterative(tree_node_t *tree, key_t query_key)
{
}*/

/*object_t *find_recursive(tree_node_t *tree, key_t query_key)
{
	if( tree->left == NULL || (tree->right == NULL && tree->key != query_key ) )
		return(NULL);
	else if (tree->right == NULL && tree->key == query_key )
		return( (object_t *) tree->left );		 
	else {
		if( query_key < tree->key )
			return( find_recursive(tree->left, query_key) );
		else
		return( find_recursive(tree->right, query_key) );
	}
}*/

int insert(tree_node_t *tree, key_t new_key, object_t *new_object)
{
	tree_node_t *tree_node;
	if( tree->left == NULL ) {
		tree->left = (tree_node_t *) new_object;
		tree->key = new_key;
		tree->right	= NULL; 
	}
	else {
		tree_node = tree;
		while( tree_node->right != NULL ) {
			if( new_key < tree_node->key )
				tree_node = tree_node->left;
			else
				tree_node = tree_node->right;
		}
		/* found the candidate leaf. Test whether key distinct */
		if( tree_node->key == new_key )
			return( -1 );

		/* key is distinct, now perform the insert */ 
		tree_node_t *old_leaf, *new_leaf;
		old_leaf = get_node();
		old_leaf->left = tree_node->left; 
		old_leaf->key = tree_node->key;
		old_leaf->right	= NULL;
		new_leaf = get_node();
		new_leaf->left = (tree_node_t *) new_object; 
		new_leaf->key = new_key;
		new_leaf->right	= NULL; 
		if( tree_node->key < new_key ) {
			tree_node->left	= old_leaf;
			tree_node->right = new_leaf;
			tree_node->key = new_key;
		} 
		else {
			tree_node->left	= new_leaf;
			tree_node->right = old_leaf;
		} 
	}
	return 0 ;
}

object_t *_delete(tree_node_t *tree, key_t delete_key)
{	
	tree_node_t *tree_node, *upper_node, *other_node;
	object_t *deleted_object;
	if( tree->left == NULL )
		return( NULL );
	else if( tree->right == NULL ) {
		if(	tree->key == delete_key ) {
			deleted_object = (object_t *) tree->left;
			tree->left = NULL;
			return( deleted_object );
		}
		else
			return( NULL );
	}
	else {
		tree_node = tree;
		while( tree_node->right != NULL ) {
			upper_node = tree_node;
			if( delete_key < tree_node->key ) {
				tree_node = upper_node->left;
				other_node = upper_node->right;
			} 
			else {
				tree_node = upper_node->right; 
				other_node = upper_node->left;
			} 
		}
		if( tree_node->key != delete_key )
			return( NULL );
		else
		{
			upper_node->key = other_node->key;
			upper_node->left = other_node->left;
			upper_node->right = other_node->right;
			deleted_object = (object_t *) tree_node->left;
			return_node( tree_node );
			return_node( other_node );
			return( deleted_object );
		}
	}
}

void remove_tree(tree_node_t *tree)
{
	tree_node_t *current_node, *tmp;
	if( tree->left == NULL )
		return_node( tree );
	else
	{
		current_node = tree;
		while(current_node->right != NULL )
		{
			if( current_node->left->right == NULL ) {
				return_node( current_node->left );
				tmp = current_node->right;
				return_node( current_node );
				current_node = tmp;
			}
			else
			{
				tmp = current_node->left;
				current_node->left = tmp->right;
				tmp->right = current_node; 
				current_node = tmp;
			}
		}
		return_node( current_node );
	}
}

/*tree_node_t *interval_find(tree_node_t *tree, key_t a, key_t b)
{	
	tree_node_t *tr_node;
	tree_node_t *node_stack[200]; int stack_p = 0;
	tree_node_t *result_list, *tmp, *tmp2;
	result_list = NULL;
	node_stack[stack_p++] = tree;
	while( stack_p > 0 ) {
		tr_node = node_stack[--stack_p];
		if( tr_node->right == NULL ) {
*/			/* reached leaf, now test */
/*	 		if( a <= tr_node->key && tr_node->key < b ) {
				tmp = get_node();			*/	/* leaf key in interval */
/*				tmp->key	= tr_node->key; */ /* copy to output list */	
/*				tmp->left = tr_node->left;	 
				tmp->right = result_list;
				result_list = tmp;
			}
		} */ /* not leaf, might have to follow down */
/*		else if ( b <= tr_node->key ) */ /* entire interval left */
/*			node_stack[stack_p++] = tr_node->left;
		else if ( tr_node->key <= a ) */ /* entire interval right*/
/*			node_stack[stack_p++] = tr_node->right;
		else {	 */ /* node key in interval, follow left and right */
/*			node_stack[stack_p++] = tr_node->left;
			node_stack[stack_p++] = tr_node->right;
		}
	}
	return( result_list );
}*/

/*void check_tree( tree_node_t *tr, int depth, int lower, int upper )
{
	if( tr->left == NULL ) {
		printf("Tree Empty\n");
		return;
	}
	if( tr->key < lower || tr->key >= upper )
		printf("Wrong Key Order \n");
	if( tr->right == NULL ) {
		if( *( (int *) tr->left) == 10*tr->key + 2 )
			printf("%d(%d)	", tr->key, depth );
		else
			printf("Wrong Object \n");
	 }
	 else {
		check_tree(tr->left, depth+1, lower, tr->key ); 
		check_tree(tr->right, depth+1, tr->key, upper ); 
	 }
}*/

text_t* create_text() {
	tree_node_t *tree_node;
	char *last_line = "\0";
	tree_node = get_node();
	tree_node->key = 0;
	tree_node->left = (tree_node_t*)last_line;
	tree_node->right = NULL;
	return (text_t*) tree_node;
}

int length_text(text_t *txt) {
	tree_node_t *tree_node = (tree_node_t*)txt;	
	int length = 0;
	while(tree_node->right!=NULL) {
		length += tree_node -> key;
		tree_node = tree_node -> left;
	}
	return length;
}

char* get_line(text_t *txt,int index) {
	int len = length_text(txt);
	tree_node_t *tree_node;
	if( txt->left == NULL )
		return(NULL);
	 else {
		tree_node = txt;
		while( tree_node->right != NULL ) {
			if( index < tree_node->key )
					tree_node = tree_node->left;
			else
				tree_node = tree_node->right;
		}
		if( tree_node->key == index )
			return( (object_t *) tree_node->left );
		else
			return( NULL );
	}

}

void append_line(text_t *txt, char *new_line) {
	
}

char* set_line(text_t *text, int index, char *new_line) {

}

void insert_line(text_t *text, int index, char *new_line){
	int length = length_text(text);
	if(index<=length) {
		tree_node_t *tree_node = text;
		tree_node_t *parent_node = NULL;
		tree_node_t *child_node;
		int tree_node_key = length + 1;
		int node_count = 0;
		/*if( tree->left == NULL ) {
			tree->left = (tree_node_t *) new_object;
			tree->key = new_key;
			tree->right	= NULL; 
		}
		else {*/
			while( tree_node->right != NULL ) {
				if( index <= tree_node_key ) {
					node_count += node_count + tree_node->key;
					child_node = tree_node -> left;
					if(parent_node != NULL) {
						tree_node->back_ptr = left;
						tree_node -> left = parent_node;
					}
				} else {
					child_node = tree_node->right;
					if(parent_node != NULL) {
						tree_node->back_ptr = right;
						tree_node -> right = parent_node;
						tree_node->key++;
					}
				}
				parent_node = tree_node;
				tree_node = child_node;
				tree_node_key = length - node_count + 1; 
			}

			tree_node_t *old_leaf, *new_leaf;
			old_leaf = get_node();
			old_leaf->left = tree_node->left;
			old_leaf->key = 0;
			old_leaf->height = 0;
			old_leaf->right	= NULL;
			new_leaf = get_node();
			new_leaf->left = (tree_node_t *) new_line;
			new_leaf->key = 0;
			new_leaf->height = 0;
			new_leaf->right	= NULL;
			if( tree_node_key <= index ) {
				if(tree_node->back_ptr = left) {
					child_node = old_leaf;
					tree_node->right = new_leaf;
				} else {
					tree_node->left	= old_leaf;
					child_node = new_leaf;
				}
			} else {
				if(tree_node->back_ptr = left) {
					child_node = new_leaf;
					tree_node->right = old_leaf;
				} else {
					tree_node->left	= new_leaf;
					child_node = old_leaf;	
				}
				tree_node->key++;
			}
			
			tree_node->height++;	
			while(tree_node->back_ptr!=null) {
				if(tree_node->back_ptr == left) {
					parent_node = tree_node->left;
					tree_node->left = child_node; 
				} else {
					parent_node = tree_node->right;
					tree_node->right = child_node;
				}

				if(tree_node->left->height-tree_node->right->height >= 2) {
					child_node = tree_node->left;
					tree_node->left = child_node->right;
					child_node->right = tree_node;	
					tree_node->height = get_max(tree_node->left->height,tree_node->right->height) +1;
					child_node->height = get_max(child_node->left->height,child_node->right->height) +1;
					child_node->key = 1+child_node->right->key;
					tree_node = child_node;					
				} else if (tree_node->right->height - tree_node->left->height >= 2){
					child_node = tree_node->right;
					tree_node->right = child_node->left;
					child_node -> left = tree_node;
					tree_node->height = get_max(tree_node->left->height,tree_node->right->height) +1;
					child_node->height = get_max(child_node->left->height,child_node->right->height) +1;
					tree_node->key = 1+tree_node->right->key;
					child_node->key = 1+child_node->right->key;
					tree_node = child_node;	
				}
				child_node = tree_node;
				tree_node = parent_node;
			}
		//}
	} else {
		append_line(text,new_line);
	}
	
}

char* delete_line(text_t *text, int index){
	
}

int main()
{
   int i, tmp; text_t *txt1, *txt2; char *c;
   printf("starting \n");
   txt1 = create_text();
   txt2 = create_text();
   append_line(txt1, "line one" );
   if( (tmp = length_text(txt1)) != 1)
   {  printf("Test 1: length should be 1, is %d\n", tmp); //exit(-1);
   }
   append_line(txt1, "line hundred" );
   insert_line(txt1, 2, "line ninetynine" );
   insert_line(txt1, 2, "line ninetyeight" );
   insert_line(txt1, 2, "line ninetyseven" );
   insert_line(txt1, 2, "line ninetysix" );
   insert_line(txt1, 2, "line ninetyfive" );
   for( i = 2; i <95; i++ )
     insert_line(txt1, 2, "some filler line between 1 and 95" );
   if( (tmp = length_text(txt1)) != 100)
   {  printf("Test 2: length should be 100, is %d\n", tmp); //exit(-1);
   }
   printf("found at line 1:   %s\n",get_line(txt1,  1));
   printf("found at line 2:   %s\n",get_line(txt1,  2));
   printf("found at line 99:  %s\n",get_line(txt1, 99));
   printf("found at line 100: %s\n",get_line(txt1,100));
   for(i=1; i<=10000; i++)
   {  if( i%2==1 )
        append_line(txt2, "A");
      else 
        append_line(txt2, "B");
   }
   if( (tmp = length_text(txt2)) != 10000)
   {  printf("Test 3: length should be 10000, is %d\n", tmp); //exit(-1);
   }
   c = get_line(txt2, 9876 );
   if( *c != 'B')
     {  printf("Test 4: line 9876 of txt2 should be B, found %s\n", c); //exit(-1);
   }
   for( i= 10000; i > 1; i-=2 )
   {  c = delete_line(txt2, i);
      if( *c != 'B')
      {  printf("Test 5: line %d of txt2 should be B, found %s\n", i, c); //exit(-1);
      }
      append_line( txt2, c );
   }
   for( i=1; i<= 5000; i++ )
   {  c = get_line(txt2, i);
      if( *c != 'A')
      {  printf("Test 6: line %d of txt2 should be A, found %s\n", i, c); //exit(-1);
      }
   }
   for( i=1; i<= 5000; i++ )
     delete_line(txt2, 1 );
   for( i=1; i<= 5000; i++ )
   {  c = get_line(txt2, i);
      if( *c != 'B')
      {  printf("Test 7: line %d of txt2 should be B, found %s\n", i, c); //exit(-1);
      }
   }
   set_line(txt1, 100, "the last line");
   for( i=99; i>=1; i-- )
     delete_line(txt1, i );
   printf("found at the last line:   %s\n",get_line(txt1,  1));
   
}


/*int main()
{
	tree_node_t *searchtree;
	char nextop;
	searchtree = create_tree();
	printf("Made Tree\n");
	printf("In the following, the key n is associated wth the objecct 10n+2\n");
	while( (nextop = getchar())!= 'q' ) {
		if( nextop == 'i' ) {
			int inskey,	*insobj, success;
			insobj = (int *) malloc(sizeof(int));
			scanf(" %d", &inskey);
			*insobj = 10*inskey+2;
			success = insert( searchtree, inskey, insobj );
			if ( success == 0 )
				 printf("insert successful, key = %d, object value = %d, \n",inskey, *insobj);
			else
				printf("insert failed, success = %d\n", success);
		}	
		if( nextop == 'f' ) {
			int findkey, *findobj;
			scanf(" %d", &findkey);
			findobj = find_iterative( searchtree, findkey);
			if( findobj == NULL )
				printf("find (iterative) failed, for key %d\n", findkey);
			else
				printf("find (iterative) successful, found object %d\n", *findobj);
			findobj = find_recursive( searchtree, findkey);
			if( findobj == NULL )
				printf("find (recursive) failed, for key %d\n", findkey);
			else
				printf("find (recursive) successful, found object %d\n", *findobj);
		}
		if( nextop == 'v' ) {
			int a, b;
			tree_node_t *results, *tmp;
			scanf(" %d %d", &a, &b);
			results = interval_find( searchtree, a, b );
			if( results == NULL )
				printf("no keys found in the interval [%d,%d[\n", a, b);
			else {
				printf("the following keys found in the interval [%d,%d[\n", a, b);
				while( results != NULL ) {
					printf("(%d,%d) ", results->key, *((int *) results->left) );
					tmp = results;
			 		results = results->right;
					return_node( tmp );
				}
				printf("\n");
			}
		}
		if( nextop == 'd' ) {
			int delkey, *delobj;
			scanf(" %d", &delkey);
			delobj = _delete( searchtree, delkey);
			if( delobj == NULL )
				printf("delete failed for key %d\n", delkey);
			else
				printf("delete successful, deleted object %d for key %d\n", *delobj, delkey);
		}
		if( nextop == '?' ) {
			printf("Checking tree\n"); 
			check_tree(searchtree,0,-1000,1000);
			printf("\n");
			if( searchtree->left != NULL )
 		 		printf("key in root is %d\n", searchtree->key );
			printf("Finished Checking tree\n"); 
		}
	}

	remove_tree( searchtree );
	printf("Removed tree.\n");
	printf("Total number of nodes taken %d, total number of nodes returned %d\n",nodes_taken, nodes_returned );
	return 0 ;
}*/

