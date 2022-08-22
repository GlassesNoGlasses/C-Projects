
#include "t_imgUtils.c"

#ifndef SIZEX
#define SIZEX 512
#define SIZEY 512
#endif

/*---------------------------------------------------------------------------*/

/**
 * This struct contains one node of the linked list, which represents a single
 * command to the Turtle. It's field should include:
 *
 *  - cmd   : A char array of size 10 holding the command name
 *
 *  - val   : An integer that stores a parameter for the command (like forward,
 *            backward and colour).
 *
 *  - next  : A pointer to a struct of the same type, this is used for the
 *            linked list
 ****/

typedef struct cmdnode
{
    char cmd[10];
    int val;
    struct cmdnode *next;
    struct cmdnode *loop_head;
} CmdNode;

/*---------------------------------------------------------------------------*/

int isValidCmd(char cmd[10])
{
    if (strcmp(cmd, "penup") == 0 || strcmp(cmd, "forward") == 0)
    {
        return 1;
    }
    else if (strcmp(cmd, "pendown") == 0 || strcmp(cmd, "backward") == 0)
    {
        return 1;
    }
    else if (strcmp(cmd, "colour") == 0 || strcmp(cmd, "right") == 0)
    {
        return 1;
    }
    else if (strcmp(cmd, "left") == 0 || strcmp(cmd, "loop") == 0)
    {
        return 1;
    }
    return 0;
}

CmdNode *newCmdNode(char cmd[10], int val)
{
    /**
     * This function allocates a new CmdNode struct and initializes it's values
     * based on the input paramaters given.
     */
    if (isValidCmd(cmd) <= 0)
    {
        printf("Invalid command.\n");
        return NULL;
    }
    CmdNode *newNode = NULL;
    newNode = (CmdNode *)calloc(1, sizeof(CmdNode));
    strcpy(newNode->cmd, cmd);
    newNode->next = NULL;
    newNode->val = val;
    newNode->loop_head = NULL;
    return newNode;
}

/*---------------------------------------------------------------------------*/

int isVal(char cmd[10])
{
    //   *  - penup          : Put the pen up (stop drawing)
    //  *  - pendown        : Put the pen down (start / continue drawing)
    //  *  - colour <val>   : Draw lines in colour <val> from now on
    //  *  - forward <val>  : Move the turtle forward <val> steps (pixels)
    //  *                     in the direction it is facing.
    //  *  - backward <val> : Same as above, except move backwards
    //  *  - right          : Turn the turtle to the right by 90 degrees
    //  *  - left           : Turn the turtle to the left by 90 degrees
    if (strcmp(cmd, "colour") == 0 || strcmp(cmd, "forward") == 0)
    {
        return 1;
    }
    else if (strcmp(cmd, "backward") == 0 || strcmp(cmd, "loop") == 0)
    {
        return 1;
    }
    return 0;
}

void checkCommandList(CmdNode *node, int count, int isVal, int isLoop)
{
    if (isLoop > 0 && isVal > 0)
    {
        printf("  %3d: %s %d\n", count, node->cmd, node->val);
    }
    else if (isLoop > 0)
    {
        printf("  %3d: %s\n", count, node->cmd);
    }
    else if (isVal > 0)
    {
        printf("%3d: %s %d\n", count, node->cmd, node->val);
    }
    else
    {
        printf("%3d: %s\n", count, node->cmd);
    }
    return;
}

void printCommandList(CmdNode *head)
{
    /**
     * This function prints out each command in the linked list one after the
     * other.
     */
    CmdNode *temp = head;
    CmdNode *loop = NULL;
    int isLoop = 1;
    int count = 0;
    while (temp != NULL)
    {
        checkCommandList(temp, count, isVal(temp->cmd), 0);
        if (strcmp(temp->cmd, "loop") == 0)
        {
            loop = temp->loop_head;
            while (loop != NULL)
            {
                count++;
                checkCommandList(loop, count, isVal(loop->cmd), isLoop);
                loop = loop->next;
            }
        }
        temp = temp->next;
        count++;
    }
    return;
}

/*---------------------------------------------------------------------------*/

void queryByCommand(CmdNode *head, char cmd[10])
{
    /**
     * This function looks for commands in the linked list that match the input
     * query.
     */
    CmdNode *temp = head;
    CmdNode *loop = NULL;
    int count = 0;
    while (temp != NULL)
    {
        if (strcmp(temp->cmd, cmd) == 0)
        {
            checkCommandList(temp, count, isVal(temp->cmd), 0);
        }
        else if (strcmp(temp->cmd, "loop") == 0)
        {
            loop = temp->loop_head;
            while (loop != NULL)
            {
                count++;
                if (strcmp(loop->cmd, cmd) == 0)
                {
                    checkCommandList(loop, count, isVal(loop->cmd), 0);
                }
                loop = loop->next;
            }
        }
        temp = temp->next;
        count++;
    }
    return;
}

/*---------------------------------------------------------------------------*/

int countCommands(CmdNode *head)
{
    /**
     * This function counts and returns the length of the linked list. 
     */
    CmdNode *temp = head;
    CmdNode *loop = NULL;
    int len = 0;
    while (temp != NULL)
    {
        if (strcmp(temp->cmd, "loop") == 0)
        {
            loop = temp->loop_head;
            while (loop != NULL)
            {
                loop = loop->next;
                len++;
            }
        }
        temp = temp->next;
        len++;
    }
    return len;
}

/*---------------------------------------------------------------------------*/

CmdNode *insertCommand(CmdNode *head, CmdNode *new_CmdNode)
{
    /**
     * This function inserts the node new_CmdNode *at the tail* of the linked
     * list.
     */
    CmdNode *temp = head;
    if (head == NULL)
    {
        head = new_CmdNode;
        return head;
    }
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = new_CmdNode;
    return head;
}

/*---------------------------------------------------------------------------*/

CmdNode *insertCommandBefore(CmdNode *head, CmdNode *new_CmdNode, int cmdNum)
{
    /**
     * This function inserts a new node *before* a given Node in the linked list.
     */
    int count = 0;
    int size = countCommands(head);
    CmdNode *temp = head;
    CmdNode *holder = NULL;
    if (size <= cmdNum)
    {
        printf("Invalid Command Number.\n");
        return head;
    }
    if (head == NULL)
    {
        head = new_CmdNode;
        return head;
    }
    if (cmdNum == 0)
    {
        new_CmdNode->next = head;
        return new_CmdNode;
    }
    while (temp != NULL)
    {
        if (count + 1 == cmdNum)
        {
            holder = temp->next;
            temp->next = new_CmdNode;
            new_CmdNode->next = holder;
            break;
        }
        temp = temp->next;
        count++;
    }
    return head;
}

/*---------------------------------------------------------------------------*/

void updateCmd(CmdNode *temp, char cmd[10])
{
    int i = 0;
    while (*(cmd + i) != '\0')
    {
        *(temp->cmd + i) = *(cmd + i);
        i++;
    }
    *(temp->cmd + i) = '\0';
}

void updateCommand(CmdNode *head, int cmdNum, char cmd[10], int val)
{
    /**
     * This function updates a specific node in the linked list based on the
     * input parameters.
     */
    if (countCommands(head) <= cmdNum)
    {
        printf("Invalid Command Number.\n");
        return;
    }
    else if (isValidCmd(cmd) < 1)
    {
        printf("Invalid command.\n");
        return;
    }
    CmdNode *temp = head;
    int count = 0;
    while (temp != NULL)
    {
        if (count == cmdNum)
        {
            updateCmd(temp, cmd);
            temp->val = val;
            return;
        }
        temp = temp->next;
        count++;
    }
    return;
}

/*---------------------------------------------------------------------------*/

CmdNode *deleteCommand(CmdNode *head, int cmdNum)
{
    /**
     * This function deletes the node from the linked list that corresponds to
     * the line number cmdNum. If there is no command with the given cmdNum, then
     * the function does nothing.
     */
    if (countCommands(head) <= cmdNum)
    {
        return head;
    }
    else if (head == NULL)
    {
        return NULL;
    }
    CmdNode *temp = head;
    CmdNode *after = head->next;
    int count = 0;
    if (cmdNum == 0 && after == NULL)
    {
        free(head);
        return NULL;
    }
    else if (cmdNum == 0)
    {
        free(head);
        return after;
    }
    while (temp != NULL)
    {
        if (count + 1 == cmdNum)
        {
            if (after->next == NULL)
            {
                free(after);
                temp->next = NULL;
            }
            else
            {
                temp->next = after->next;
                free(after);
            }
            break;
        }
        temp = temp->next;
        after = temp->next;
        count++;
    }
    return head;
}

/*---------------------------------------------------------------------------*/

CmdNode *deleteCommandList(CmdNode *head)
{
    /**
     * This function deletes the linked list of commands, releasing all the
     * memory allocated to the nodes in the linked list.
     */

    CmdNode *temp = NULL;
    CmdNode *after = NULL;
    CmdNode *loop = NULL;
    temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->cmd, "loop") == 0)
        {
            loop = temp->loop_head;
            loop = deleteCommandList(loop);
        }
        after = temp->next;
        free(temp);
        temp = after;
    }

    return NULL;
}

/*---------------------------------------------------------------------------*/
void changeDirection(char cmd[10], int *pX, int *pY)
{
    if (strcmp(cmd, "right") == 0)
    {
        if (*(pX) == 1)
        {
            *(pX) = 0;
            *(pY) = -1;
        }
        else if (*(pX) == -1)
        {
            *(pX) = 0;
            *(pY) = 1;
        }
        else if (*(pY) == -1)
        {
            *(pY) = 0;
            *(pX) = -1;
        }
        else
        {
            *(pY) = 0;
            *(pX) = 1;
        }
        return;
    }
    if (*(pX) == 1)
    {
        *(pX) = 0;
        *(pY) = 1;
    }
    else if (*(pX) == -1)
    {
        *(pX) = 0;
        *(pY) = -1;
    }
    else if (*(pY) == -1)
    {
        *(pY) = 0;
        *(pX) = 1;
    }
    else
    {
        *(pY) = 0;
        *(pX) = -1;
    }
    return;
}

int checkPosition(int position, char z)
{ // make sure size x and y are appropriate
    if (z == 'x')
    {
        if (position < 0)
        {
            return 0;
        }
        else if (position >= SIZEX)
        {
            return SIZEX - 1;
        }
    }
    else if (z == 'y')
    {
        if (position < 0)
        {
            return 0;
        }
        else if (position >= SIZEY)
        {
            return SIZEY - 1;
        }
    }
    return position;
}

int checkColour(int val)
{
    if (val < 0)
    {
        return 0;
    }
    else if (val > 255)
    {
        return 255;
    }
    return val;
}

void move(CmdNode *node, int *endX, int *endY, int xPos, int yPos, int yOrient, int xOrient, int direction)
{
    if (direction == 1)
    {
        if (yOrient == 0 && xOrient == 1)
        {
            *(endX) = checkPosition(node->val + xPos, 'x');
        }
        else if (yOrient == 0 && xOrient == -1)
        {
            *(endX) = checkPosition(xPos - node->val, 'x');
        }
        else if (yOrient == 1 && xOrient == 0)
        {
            *(endY) = checkPosition(yPos - node->val, 'y');
        }
        else
        {
            *(endY) = checkPosition(yPos + node->val, 'y');
        }
        return;
    }
    if (yOrient == 0 && xOrient == 1)
    {
        *(endX) = checkPosition(xPos - node->val, 'x');
    }
    else if (yOrient == 0 && xOrient == -1)
    {
        *(endX) = checkPosition(node->val + xPos, 'x');
    }
    else if (yOrient == 1 && xOrient == 0)
    {
        *(endY) = checkPosition(node->val + yPos, 'y');
    }
    else
    {
        *(endY) = checkPosition(yPos - node->val, 'y');
    }
    return;
}

void run(Image *im, CmdNode *head, int *endX, int *endY)
{
    /**
     * This function runs the list of commands to move the turtle around and draw
     * the image, and returns the final position of the turtle in the variables
     * endX and endY.
     *
     * --------------------------------------------------------------------------
     *
     * NOTE: In the image we work with, the top-left pixel is (0,0),
     *       - x increases as you go right, up till SIZEX-1
     *       - y increases as you go down, up till SIZEY-1
     *
     *                 (0,0)                 (SIZEX-1, 0)
     *                   x------------------------x
     *                   |                        |
     *                   |                        |
     *                   |                        |
     *                   |                        |
     *                   |          IMAGE         |
     *                   |                        |
     *                   |                        |
     *                   |                        |
     *                   |                        |
     *                   |                        |
     *                   x------------------------x
     *             (0, SIZEY-1)            (SIZEX-1, SIZEY-1)
     *
     * The image is in grayscale (black and white), so the colours are numbers
     * from [0-255] where 0 is black, 255 is white, and the values in between
     * are varying levels of gray.
     * --------------------------------------------------------------------------
     *
     * Here's the setup - There is a turtle (with a pen) that walks along the
     * image. When the pen is down (on the paper), it draws a line along the path
     * that it walks on.
     *
     * The turtle initially starts at pixel (0, 0) [at the top left],
     * facing right (in the positive x direction). The pen starts off
     * as `down`, with the default colour being black (0).

     *
     *  - penup          : Put the pen up (stop drawing)
     *  - pendown        : Put the pen down (start / continue drawing)
     *  - colour <val>   : Draw lines in colour <val> from now on
     *  - forward <val>  : Move the turtle forward <val> steps (pixels)
     *                     in the direction it is facing.
     *  - backward <val> : Same as above, except move backwards
     *  - right          : Turn the turtle to the right by 90 degrees
     *  - left           : Turn the turtle to the left by 90 degrees
     *
     */
    *(endX) = 0;
    *(endY) = 0;
    if (head == NULL)
    {
        return;
    }
    // controls up, down, left, right;
    int xOrient = 1;
    int yOrient = 0;
    // controls if pen is down (1) and colour value;
    int pen = 1;
    int colour = 0;
    int *pX = &xOrient;
    int *pY = &yOrient;
    // current (x,y) values
    int xPos = 0;
    int yPos = 0;
    int loopCount;
    CmdNode *temp = head;
    CmdNode *loop = NULL;

    while (temp != NULL)
    {
        if (strcmp(temp->cmd, "penup") == 0)
        {
            pen = 0;
        }
        else if (strcmp(temp->cmd, "pendown") == 0)
        {
            pen = 1;
        }
        else if (strcmp(temp->cmd, "forward") == 0)
        {
            move(temp, endX, endY, xPos, yPos, yOrient, xOrient, 1);
            if (pen > 0)
            {
                drawLine(im, xPos, yPos, *(endX), *(endY), colour);
            }
            xPos = *(endX);
            yPos = *(endY);
        }
        else if (strcmp(temp->cmd, "backward") == 0)
        {
            move(temp, endX, endY, xPos, yPos, yOrient, xOrient, 0);
            if (pen > 0)
            {
                drawLine(im, xPos, yPos, *(endX), *(endY), colour);
            }
            xPos = *(endX);
            yPos = *(endY);
        }
        else if (strcmp(temp->cmd, "right") == 0 || strcmp(temp->cmd, "left") == 0)
        {
            changeDirection(temp->cmd, pX, pY);
        }
        else if (strcmp(temp->cmd, "colour") == 0)
        {
            colour = checkColour(temp->val);
        }
        else if (strcmp(temp->cmd, "loop") == 0)
        {
            loopCount = temp->val;
            while (loopCount > 0)
            {
                loop = temp->loop_head;
                while (loop != NULL)
                {
                    if (strcmp(loop->cmd, "penup") == 0)
                    {
                        pen = 0;
                    }
                    else if (strcmp(loop->cmd, "pendown") == 0)
                    {
                        pen = 1;
                    }
                    else if (strcmp(loop->cmd, "forward") == 0)
                    {
                        move(loop, endX, endY, xPos, yPos, yOrient, xOrient, 1);
                        if (pen > 0)
                        {
                            drawLine(im, xPos, yPos, *(endX), *(endY), colour);
                        }
                        xPos = *(endX);
                        yPos = *(endY);
                    }
                    else if (strcmp(loop->cmd, "backward") == 0)
                    {
                        move(loop, endX, endY, xPos, yPos, yOrient, xOrient, 0);
                        if (pen > 0)
                        {
                            drawLine(im, xPos, yPos, *(endX), *(endY), colour);
                        }
                        xPos = *(endX);
                        yPos = *(endY);
                    }
                    else if (strcmp(loop->cmd, "right") == 0 || strcmp(loop->cmd, "left") == 0)
                    {
                        changeDirection(loop->cmd, pX, pY);
                    }
                    else if (strcmp(loop->cmd, "colour") == 0)
                    {
                        colour = checkColour(loop->val);
                    }
                    loop = loop->next;
                }
                loopCount--;
            }
        }
        temp = temp->next;
    }
    return;
}

void insertCommandLoop(CmdNode *loop, CmdNode *new_CmdNode)
{
    CmdNode *temp = loop->loop_head;
    if (temp == NULL)
    {
        loop->loop_head = new_CmdNode;
        return;
    }
    loop->loop_head = insertCommand(loop->loop_head, new_CmdNode);
    return;
}

/*---------------------------------------------------------------------------*/