
#include "imgUtils.c"

/**
 * This is the structure we are going to use to store each individual node of
 * the BST. Remember that each Quad corresponds to a rectangular area on the
 * image:
 *
 *                (tx,ty)         w
 *                   x-------------------------
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                 h |          Quad          |
 *                   |    key = tx+(ty*sx)    |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   -------------------------x
 *                                       (tx + w, ty + h)
 *
 */
typedef struct quad
{
  int tx, ty; // The (x,y) coordinates of the top-left pixel in the quad
  int w;      // How many pixels wide the quad is
  int h;      // How many pixels high the quad is

  int sx; // Width of the original image, this is needed for the key.
          // This *MUST* be the same for all nodes in the BST

  int key; // A unique identifier (remember we discussed BST nodes
           // should have unique keys to identify each node). The
           // key identifier here will be created as:
           //       key = tx + (ty * sx)
           // This means that only one quad can start at a specific
           // pixel.

  int wsplit; // 1 if this quad is supposed to be split along the width
              // 0 if this quad is supposed to be split along the height

  struct quad *left;
  struct quad *right;
} Quad;

///////////////////////////////////////////////////////////////////////////////

Quad *new_Quad(int tx, int ty, int w, int h, int wsplit, int sx)
{
  /**
   * This function creates and initializes a new Quad for a rectanglecstarting
   * at (tx, ty) with a width 'w' and height 'h'. The width ofcthe image in
   * which this rectangle exists is 'sx', use this to computecthe key as:
   *
   * 		key = tx + (ty * sx)
   */
  Quad *newNode = NULL;
  newNode = (Quad *)calloc(1, sizeof(Quad));
  newNode->tx = tx;
  newNode->ty = ty;
  newNode->w = w;
  newNode->h = h;
  newNode->wsplit = wsplit;
  newNode->sx = sx;
  newNode->key = tx + (ty * sx);
  newNode->right = NULL;
  newNode->left = NULL;
  return newNode;
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_insert(Quad *root, Quad *new_node)
{
  /**
   * This function inserts a new Quad node into the BST rooted atc'root'. The
   * new_node must already be initialized with validcdata, and must have a
   * unique key.
   *
   * printf("Duplicate Quad (tx,ty,sx)=%d,%d, %d, was ignored\n",....);
   */
  if (root == NULL)
  {
    return new_node;
  }
  if (new_node == NULL) {
    return root;
  }
  if (root->key == new_node->key)
  {
    printf("Duplicate Quad (tx,ty,sx)=%d,%d, %d, was ignored\n", new_node->tx, new_node->ty, new_node->sx);
    return root;
  }
  else if (root->key > new_node->key)
  {
    root->left = BST_insert(root->left, new_node);
  }
  else
  {
    root->right = BST_insert(root->right, new_node);
  }
  return root;
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_search(Quad *root, int tx, int ty)
{
  /**
   * This function searches the BST for a Quad at the speficied position. If
   * found, it must return a pointer to the quad that contains it.
   */
  if (root == NULL)
  {
    return NULL;
  }
  int searchKey = tx + (ty * root->sx);
  if (root->key == searchKey)
  {
    return root;
  }
  else if (root->key > searchKey)
  {
    return BST_search(root->left, tx, ty);
  }
  return BST_search(root->right, tx, ty);
}

///////////////////////////////////////////////////////////////////////////////

Quad *find_successor(Quad *right_child)
{
  /**
   * This function finds the successor of a Quad node by searching the right
   * subtree for the node that is most to the left (that will be the node
   * with the smallest key in that subtree)
   */
  if (right_child == NULL) {
    return NULL;
  }
  Quad *temp = right_child;
  while (temp->left != NULL) {
    temp = temp->left;
  }
  return temp;
}

///////////////////////////////////////////////////////////////////////////////
void copyData(Quad *dest, Quad *copy)
{
  dest->h = copy->h;
  dest->key = copy->key;
  dest->tx = copy->tx;
  dest->ty = copy->ty;
  dest->w = copy->w;
  dest->wsplit = copy->wsplit;
  return;
};

Quad *BST_delete(Quad *root, int tx, int ty)
{
  /**
   * Deletes from the BST a Quad at the specified position.
   */
  Quad *temp = NULL;
  Quad *smallest = NULL;
  if (root == NULL)
  {
    return NULL;
  }
  int searchKey = tx + (ty * root->sx);

  if (root->key == searchKey)
  {
    if (root->left == NULL && root->right == NULL)
    {
      free(root);
      return NULL;
    }
    else if (root->right == NULL)
    {
      temp = root->left;
      free(root);
      return temp;
    }
    else if (root->left == NULL)
    {
      temp = root->right;
      free(root);
      return temp;
    }
    smallest = find_successor(root->right);
    copyData(root, smallest);
    root->right = BST_delete(root->right, smallest->tx, smallest->ty);
  }
  if (root->key > searchKey) {
    root->left = BST_delete(root->left, tx, ty);
  }
  else if (root->key < searchKey) {
    root->right = BST_delete(root->right, tx ,ty);
  }
  return root;
}

///////////////////////////////////////////////////////////////////////////////

Quad *delete_BST(Quad *root)
{
  /**
   * This function deletes the BST and frees all memory used for nodes in it.
   */
  if (root == NULL) {
    return NULL;
  }
  delete_BST(root->left);
  delete_BST(root->right);
  free(root);
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void BST_inorder(Quad *root, int depth)
{
  /**
   * This function performs an in-order traversal of the BST and prints out the
   * information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   */
  if (root == NULL) {
    return;
  }
  BST_inorder(root->left, depth + 1);
  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx, root->ty, root->w, root->h, root->wsplit);
  BST_inorder(root->right, depth + 1);
  return;
}

///////////////////////////////////////////////////////////////////////////////

void BST_preorder(Quad *root, int depth)
{
  /**
   * This function performs a pre-order traversal of the BST and prints out the
   * information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   */
  if (root == NULL) {
    return;
  }
  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx, root->ty, root->w, root->h, root->wsplit);
  BST_preorder(root->left, depth + 1);
  BST_preorder(root->right, depth + 1);
  return;
}

///////////////////////////////////////////////////////////////////////////////

void BST_postorder(Quad *root, int depth)
{
  /**
   * This function performs a post-order traversal of the BST and prints out
   * the information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   */

  if (root == NULL) {
    return;
  }
  BST_postorder(root->left, depth + 1);
  BST_postorder(root->right, depth + 1);
  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx, root->ty, root->w, root->h, root->wsplit);
  return;
}

///////////////////////////////////////////////////////////////////////////////
// NOTE: For the remaining functions, you may assume the following:          //
//       (1) All the Quads are valid (None of them go outside the image)     //
//       (2) They don't overlap  (A pixel will not be in multiple Quads)     //
///////////////////////////////////////////////////////////////////////////////

int checkSum(int colour) {
  if (colour < 0) {
    return 0;
  }
  else if (colour > 255) {
    return 255;
  }
  return colour;
}

int get_colour(Image *im, Quad *q)
{
  /**
   * Given an image 'im' and a Quad 'q', get the colour we should be assigning
   * to the pixels that are in it, and return it. For the sake of this
   * assignment, we will say this is *average* colour of all the pixels in
   * the quad.
   *
   * The pixel data is stored in a one dimensional array called 'data' in the
   * image struct. Make sure you look at the definition of this to understand
   * how the image is stored. Remember that the pixel data is stored in
   * row-major order, so to get the colour for pixel (x,y) you will look at the
   * index
   *
   *                           x + (y * sx)
   *
   * of the array.
   *
   * TODO: Implement this function. You should not be getting any values
   *       outside the range of the pixels [0-255] if you have implemented
   *       this correctly.
   */
  if (im == NULL || q == NULL) {
    return 0;
  }
  int start = q->tx + (q->ty * im->sx);
  int count = 0;
  int sum = 0;
  for (int i = 0; i < q->h; i++) {
    for (int j = start; j < start + q->w; j++) {
      sum = sum + checkSum(im->data[j + i * im->sx]);
      count++;
    }
  }
  return sum / count;
}

///////////////////////////////////////////////////////////////////////////////

int simHelp(Image *im, Quad *q) {
  int start = q->tx + (q->ty * im->sx);
  int current;
  int max = -1;
  int min = 256;
  for (int i = 0; i < q->h; i++) {
    for (int j = start; j < start + q->w; j++) {
      current = checkSum(im->data[j + i * im->sx]);
      if (current > max) {
        max = current;
      }
      if (min > current) {
        min = current;
      }
    }
  }
  return max - min;
}

int similar(Image *im, Quad *q, int threshold)
{
  /**
   * Given an image 'im', check if the colours in the area corresponding to the
   * Quad 'q' are all similar. If not, we will have to split it. For the
   * purpose of this assigment, we say the colours in a Quad are similar if
   *
   *          maxCol - minCol <= threshold
   *
   * where maxCol and minCol are the maximum and minimum values respectively
   * of the pixel colours in the Quad. The threshold is a parameter. This
   * function should return a 0 if the pixels are not similar enough and the
   * Quad needs to be split, and 1 otherwise.
   *
   */
  if (q == NULL || im == NULL) {
    return 0;
  }
  // printf("%d\n", simHelp(im, q));
  if (simHelp(im, q) > threshold) {
    return 0;
  }
  return 1;
}

///////////////////////////////////////////////////////////////////////////////

Quad *divide(Image *im, Quad *root, int threshold) {
  if (similar(im, root, threshold) == 0) {
    Quad *B = NULL;
    if (root->wsplit == 1) {
      if (root->w % 2 == 1) {
        B = new_Quad(root->tx + root->w/2, root->ty, root->w/2 + 1, root->h, 0, root->sx);
      }
      else {
        B = new_Quad(root->tx + root->w/2, root->ty, root->w/2, root->h, 0, root->sx);
      }
      root->w = root->w / 2;
      root->wsplit = 0;
      return B;
    }
    if (root->h % 2 == 1) {
      B = new_Quad(root->tx, root->ty + root->h/2, root->w, root->h/2 + 1, 1, root->sx);
    }
    else {
      B = new_Quad(root->tx, root->ty + root->h/2, root->w, root->h/2, 1, root->sx);
    }
    root->h = root->h / 2;
    root->wsplit = 1;
    return B;
  }
  return NULL;
}

void splitHelper(Image *im, Quad *root, Quad *original, int threshold) {
  if (original == NULL) {
    return;
  }
  else if (original->key == root->key) {
    return;
  }
  splitHelper(im, root, original->left, threshold);
  splitHelper(im, root, original->right, threshold);
  Quad *BNode = divide(im, original, threshold);
  Quad *copyNode = new_Quad(original->tx, original->ty, original->w, original->h, original->wsplit, original->sx);
  root = BST_insert(root, copyNode);
  if (BNode != NULL) {
    root = BST_insert(root, BNode);
  }
  return;
}

Quad *split_tree(Image *im, Quad *root, int threshold)
{
  /**
   * This function traverses the BST, and for each existing Quad, checks if
   * the pixels in the quad are of a similarcolour using the similar() function
   * and the given threshold. If they are not, then the Quad needs to be split
   * into 2 Quads(which will hopefully have pixels that are more similar to
   * each other).
   *
   *    - If wsplit = 1, then we split it along the width (ie, we will now have
   *                     2 quads with the same heightand half the width as the
   *                     original one)
   *
   *    - If wsplit = 0, then we split along the height.
   *
   */
  if (root == NULL) {
    return NULL;
  }
  Quad *head = NULL;
  Quad *headBNode = NULL;
  headBNode = divide(im, root, threshold);
  head = new_Quad(root->tx, root->ty, root->w, root->h, root->wsplit, root->sx);
  head = BST_insert(head, headBNode);
  splitHelper(im, head, root->left, threshold);
  splitHelper(im, head, root->right, threshold);
  delete_BST(root);

  return head;
}

///////////////////////////////////////////////////////////////////////////////

void drawOutline(Image *im, Quad *root, unsigned char col)
{
  /**
   * Given an image 'im' and a BST rooted at 'root', traverse through each quad
   * and draw an outline for it. The outline consists of the outermost pixels
   * of the Quad (ie, the top and bottom rows, and the leftmost and rightmost
   * columns).
   *
   */
  if (root == NULL) {
    return;
  }
  int start = root->tx + (root->ty * im->sx);
  for (int i = 0; i < root->h; i++) {
    for (int j = start; j < start + root->w; j++) {
      if (i == 0 || i == root->h - 1) {
        im->data[j + i * im->sx] = col;
      }
      else if (j == start || j == start + root->w -1) {
        im->data[j + i * im->sx] = col;
      }
    }
  }
  drawOutline(im, root->left, col);
  drawOutline(im, root->right, col);
  return;
}

///////////////////////////////////////////////////////////////////////////////

void save_Quad(Image *im, Quad *root)
{
  /**
   * Given an image 'im' and a BST rooted at 'root', traverse through each
   * quad, and set all the pixels in the corresponding area to the expected
   * colour of the quad computed by your function get_colour().
   */
  if (root == NULL || im == NULL) {
    return;
  }
  int colour = get_colour(im, root);
  int start = root->tx + (root->ty * im->sx);
  for (int i = 0; i < root->h; i++) {
    for (int j = start; j < start + root->w; j++) {
      im->data[j + i * im->sx] = colour;
    }
  }
  save_Quad(im, root->left);
  save_Quad(im, root->right);
  return;
}

///////////////////////////////////////////////////////////////////////////////
