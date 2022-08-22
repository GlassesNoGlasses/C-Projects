
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024
#define MAT_SIZE 3 // A small graph

typedef struct user_struct
{
  char name[MAX_STR_LEN];
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
} User;

typedef struct friend_node_struct
{
  User *user;
  struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
} BrandNode;

// Adjacency List 
FriendNode *allUsers;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Checks if a user is inside a FriendNode LL.
 **/
bool in_friend_list(FriendNode *head, User *node)
{
  for (FriendNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->user->name, node->name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Checks if a brand is inside a BrandNode LL.
 **/
bool in_brand_list(BrandNode *head, char *name)
{
  for (BrandNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->brand_name, name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Inserts a User into a FriendNode LL in sorted position. If the user
 * already exists, nothing is done. Returns the new head of the LL.
 **/
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (in_friend_list(head, node))
  {
    printf("User already in list\n");
    return head;
  }
  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
  {
    return fn;
  }

  if (strcmp(head->user->name, node->name) > 0)
  {
    fn->next = head;
    return fn;
  }

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;
  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Inserts a brand into a BrandNode LL in sorted position. If the brand
 * already exists, nothing is done. Returns the new head of the LL.
 **/
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (in_brand_list(head, node))
  {
    printf("Brand already in list\n");
    return head;
  }
  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->brand_name, node) > 0)
  {
    fn->next = head;
    return fn;
  }

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;
  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Deletes a User from FriendNode LL. If the user doesn't exist, nothing is
 * done. Returns the new head of the LL.
 **/
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (!in_friend_list(head, node))
  {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0)
  {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Deletes a brand from BrandNode LL. If the user doesn't exist, nothing is
 * done. Returns the new head of the LL.
 **/
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (!in_brand_list(head, node))
  {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0)
  {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Prints out the user data.
 **/
void print_user_data(User *user)
{
  printf("User name: %s\n", user->name);
  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next)
  {
    printf("   %s\n", f->user->name);
  }
  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next)
  {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Get the index into brand_names for the given brand name. If it doesn't
 * exist in the array, return -1
 **/
int get_brand_index(char *name)
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(brand_names[i], name) == 0)
    {
      return i;
    }
  }
  printf("brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Print out brand name, index and similar brands.
 **/
void print_brand_data(char *brand_name)
{
  int idx = get_brand_index(brand_name);
  if (idx < 0)
  {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }
  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);
  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
    {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
  // Read the file
  char buff[MAX_STR_LEN];
  FILE *f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char *line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (i == MAT_SIZE - 1)
    {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++)
  {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++)
    {
      int value = (int)buff[y * 2];
      if (value == 48)
      {
        value = 0;
      }
      else
      {
        value = 1;
      }
      brand_adjacency_matrix[x][y] = value;
    }
  }
}


int checkName(char *name)
{
  int len = strlen(name);
  for (int i = 0; i < len; i++)
  {
    if (name[i] != ' ' || name[i] != '\0')
    {
      return 1;
    }
  }
  return 0;
}

User *create_user(char *name)
{
  if (strcmp("", name) == 0 || checkName(name) == 0)
  {
    return NULL;
  }
  User *newUser = (User *)calloc(1, sizeof(User));
  strcpy(newUser->name, name);
  newUser->friends = NULL;
  newUser->brands = NULL;
  newUser->visited = false;
  allUsers = insert_into_friend_list(allUsers, newUser);
  return newUser;
}


BrandNode *delete_brand_from_user(BrandNode *head)
{
  if (head == NULL)
  {
    return head;
  }
  BrandNode *current = head;
  BrandNode *after = NULL;
  while (current != NULL)
  {
    after = current->next;
    free(current);
    current = after;
  }
  return NULL;
}

int delete_user(User *user)
{
  if (user == NULL)
  {
    return -1;
  }
  user->brands = delete_brand_from_user(user->brands);
  FriendNode *temp = user->friends;
  FriendNode *holder = NULL;
  while (temp != NULL)
  {
    if (in_friend_list(temp->user->friends, user)) {
      temp->user->friends = delete_from_friend_list(temp->user->friends, user);
    }
    temp = temp->next;
  }
  temp = user->friends;
  while (temp != NULL) {
    holder = temp->next;
    free(temp);
    temp = holder;
  }
  allUsers = delete_from_friend_list(allUsers, user);
  free(user);
  return 0;
}


int add_friend(User *user, User *friend)
{
  if (user == NULL || friend == NULL)
  {
    return -1;
  }
  else if (strcmp(user->name, friend->name) == 0) {
    return -1;
  }
  else if (in_friend_list(user->friends, friend)) {
    return -1;
  }
  user->friends = insert_into_friend_list(user->friends, friend);
  friend->friends = insert_into_friend_list(friend->friends, user);
  return 0;
}


int remove_friend(User *user, User *friend)
{
  if (user == NULL || friend == NULL || user->friends == NULL || friend->friends == NULL)
  {
    return -1;
  }
  else if (!in_friend_list(user->friends, friend))
  {
    return -1;
  }
  user->friends = delete_from_friend_list(user->friends, friend);
  friend->friends = delete_from_friend_list(friend->friends, user);
  return 0;
}


int follow_brand(User *user, char *brand_name)
{
  int brand_i = get_brand_index(brand_name);
  if (user == NULL || brand_i == -1)
  {
    return -1;
  }
  else if (in_brand_list(user->brands, brand_name))
  {
    return -1;
  }
  user->brands = insert_into_brand_list(user->brands, brand_name);
  return 0;
}


int unfollow_brand(User *user, char *brand_name)
{
  if (user == NULL || get_brand_index(brand_name) == -1)
  {
    return -1;
  }
  user->brands = delete_from_brand_list(user->brands, brand_name);
  return 0;
}


int get_mutual_friends(User *a, User *b)
{
  if (a == NULL || b == NULL || a->friends == NULL || b->friends == NULL)
  {
    return 0;
  }
  int num_friends = 0;
  FriendNode *a_temp = a->friends;
  FriendNode *b_temp = b->friends;
  while (a_temp != NULL)
  {
    while (b_temp != NULL)
    {
      if (strcmp(a_temp->user->name, b_temp->user->name) == 0)
      {
        num_friends++;
      }
      b_temp = b_temp->next;
    }
    b_temp = b->friends;
    a_temp = a_temp->next;
  }
  return num_friends;
}


void massReset() {
  FriendNode *temp = allUsers;
  while (temp != NULL) {
    if (temp->user->visited) {
      temp->user->visited = false;
    }
    temp = temp->next;
  }
  return;
}

int numUsers(){
  int count = 0;
  FriendNode *temp = allUsers;
  while(temp != NULL) {
    count++;
    temp = temp->next;
  }
  return count;
}

int checkQueue(User **queue, User *user, int total) {
  int i = 0;
  while (queue[i] != NULL && i < total) {
    if (strcmp(user->name, queue[i]->name) == 0) {
      return -1;
    }
    i++;
  }
  if (i == total -1) {
    return -1;
  }
  return i;
}

int addToQueue(FriendNode *currentFriends, User **queue, int total) {
  FriendNode *temp = currentFriends;
  int num;
  int count = 0;
  while (temp != NULL) {
    if (!temp->user->visited) {
      num = checkQueue(queue, temp->user, total);
      if (num != -1) {
        temp->user->visited = true;
        queue[num] = temp->user;
        count++;
      }
    }
    temp = temp->next;
  }
  return count;
}

User* removeFirstFromQueue(User **queue, int total) {
  int i = 0;
  User *holder = queue[0];
  while (i + 1 < total) {
    queue[i] = queue[i + 1];
    i++;
  }
  return holder;
}

void fillQueueLength(int *queueLength, int total) {
  for(int i = 0; i < total; i++) {
    queueLength[i] = 0;
  }
  return;
}

int bfs(FriendNode *currentFriends, User** queue, User *b, int total) {
  int i = 1;
  int count = 1;
  int final = -1;
  int queueLength[total];
  int segment;
  fillQueueLength(queueLength, total);
  User *current = NULL;
  segment = addToQueue(currentFriends, queue, total);
  queueLength[0] = segment;

  while(queue[0] != NULL) {
    current = removeFirstFromQueue(queue, total);
    if(current != NULL) {
      if (segment == 0) {
        count++;
        segment = queueLength[i];
        i++;
      }
      if (in_friend_list(current->friends, b)) {
        final = count + 1;
        break;
      }
      queueLength[i] = queueLength[i] + addToQueue(current->friends, queue, total);
    }
    segment--;
  }
  return final;
}

int get_degrees_of_connection(User *a, User *b)
{
  if (a == NULL || b == NULL || a->friends == NULL || b->friends == NULL)
  {
    return -1;
  }
  else if (strcmp(a->name, b->name) == 0) {
    return 0;
  }
  else if (in_friend_list(a->friends, b)) {
    return 1;
  }
  int total = numUsers();
  User *queue[total];
  for (int i = 0; i < total; i++) {
    queue[i] = NULL;
  }
  massReset();
  a->visited = true;
  return bfs(a->friends, queue, b, total);
}


void connect_similar_brands(char *brandNameA, char *brandNameB)
{
  int a_idx = get_brand_index(brandNameA);
  int b_idx = get_brand_index(brandNameB);
  if (a_idx == -1 || b_idx == -1)
  {
    return;
  }
  brand_adjacency_matrix[a_idx][b_idx] = 1;
  brand_adjacency_matrix[b_idx][a_idx] = 1;
  return;
}


void remove_similar_brands(char *brandNameA, char *brandNameB)
{
  int a_idx = get_brand_index(brandNameA);
  int b_idx = get_brand_index(brandNameB);
  if (a_idx == -1 || b_idx == -1)
  {
    return;
  }
  brand_adjacency_matrix[a_idx][b_idx] = 0;
  brand_adjacency_matrix[b_idx][a_idx] = 0;
  return;
}


int get_sim_brands_user(User *user, User *other)
{
  if (user == NULL || other == NULL)
  {
    return 0;
  }
  int sim_brands = 0;
  BrandNode *brand = other->brands;
  while (brand != NULL && user->brands != NULL)
  {
    if (in_brand_list(user->brands, brand->brand_name))
    {
      sim_brands++;
    }
    brand = brand->next;
  }
  return sim_brands;
}

User *get_suggested_friend(User *user)
{
  if (user == NULL)
  {
    return NULL;
  }
  User *most_sim = NULL;
  FriendNode *temp = allUsers;
  int sim_brands = 0;
  int current_user = 0;
  while (temp != NULL)
  {
    current_user = get_sim_brands_user(user, temp->user);
    if (current_user >= sim_brands && strcmp(temp->user->name, user->name) != 0 && !in_friend_list(user->friends, temp->user))
    {
      if (current_user == sim_brands && most_sim != NULL && strcmp(temp->user->name, most_sim->name) > 0) {
        most_sim = temp->user;
      }
      else {
        sim_brands = current_user;
        most_sim = temp->user;
      }
    }
    temp = temp->next;
  }
  return most_sim;
}

// Adds a suggested friend based off of the number of similar brands shared
int add_suggested_friends(User *user, int n)
{
  if (user == NULL || n <= 0)
  {
    return 0;
  }
  int count = 0;
  User *toAdd = get_suggested_friend(user);
  while (toAdd != NULL && count < n)
  {
    add_friend(user, toAdd);
    toAdd = get_suggested_friend(user);
    count++;
  }
  return count;
}

int sim_brand_num(BrandNode *head, char *brand)
{
  if (head == NULL)
  {
    return 0;
  };
  int num = 0;
  int brand_idx = get_brand_index(brand);
  int u_brand_idx;
  BrandNode *temp = head;
  while (temp != NULL)
  {
    u_brand_idx = get_brand_index(temp->brand_name);
    if (brand_adjacency_matrix[u_brand_idx][brand_idx] == 1)
    {
      num++;
    }
    temp = temp->next;
  };
  return num;
}

bool in_brand_array(char brands_to_add[MAT_SIZE][MAX_STR_LEN], char* brand) {
  for (int i = 0; i < MAT_SIZE; i++) {
    if (strcmp(brand, brands_to_add[i]) == 0) {
      return true;
    }
  }
  return false;
}

void fill_brand_rec(User *user, char brands_to_add[MAT_SIZE][MAX_STR_LEN])
{
  if (user == NULL)
  {
    return;
  }
  BrandNode *temp = user->brands;
  char *newBrand = NULL;
  int sim_brands = -1;
  int num;
  for (int j = 0; j < MAT_SIZE; j++)
  {
    if (!in_brand_list(temp, brand_names[j]) && strcmp(brand_names[j], "") != 0 && !in_brand_array(brands_to_add, brand_names[j]))
    {
      num = sim_brand_num(temp, brand_names[j]);
      if (num > sim_brands) {
        sim_brands = num;
        newBrand = brand_names[j];
      }
      else if (num == sim_brands && strcmp(brand_names[j], newBrand) > 0) {
        newBrand = brand_names[j];
      }
    }
  }
  if (newBrand != NULL) {
    for(int k = 0; k < MAT_SIZE; k++) {
      if (strcmp(brands_to_add[k], "") == 0) {
        strcpy(brands_to_add[k], newBrand);
        break;
      }
    }
  }
  return;
}

// follows n suggested brands based off of the brands the user currently follows
// performed in reverse-alphanumeric order
int follow_suggested_brands(User *user, int n)
{
  if (user == NULL || n <= 0)
  {
    return 0;
  }
  int count = 0;
  char brands_to_add[MAT_SIZE][MAX_STR_LEN];
  for (int i = 0; i < MAT_SIZE; i++)
  {
    strcpy(brands_to_add[i], "");
  }
  for (int j = 0; j < n; j++) {
    fill_brand_rec(user, brands_to_add);
  }
  while (n > 0 && strcmp(brands_to_add[count], "") != 0) {
    user->brands = insert_into_brand_list(user->brands, brands_to_add[count]);
    n--;
    count++;
  }
  return count;
}
