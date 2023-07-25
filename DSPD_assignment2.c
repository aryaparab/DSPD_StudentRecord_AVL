#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node {
	char name[50];
	char dept[5];
	int dept_index;
	int sem;
	char rollno[10];
	int rollnumber;
    int mcc_attended;
    int percent_attendance;
    int attended[5];
	int conducted[5];
	char exam_applied[5];
	char fee_status_str[10];
	
    struct Node* left;
    struct Node* right;
    int height;
};


struct max_dept_defaulters {
	int dept[5];
    //this contans array of 5 elements and each represent a department 
    //once u find a defaulter you will go in respective dept ondex and increment
};
void display(struct Node *head);

struct Node* newNode(struct Node* item);
struct Node* insert(struct Node* node, struct Node* key);
void updateMccPercentAttend(struct Node* temp);
void feeStatusPendingAttend75(struct Node* head);
void notAppliedExam(struct Node* studentList);
void formEligibleList(struct Node *root, FILE * f_ptr);
void formLessThan75AttendanceList(struct Node *root, FILE * f_ptr);
struct Node* deleteNode(struct Node* root, char key[10]);
void copy(struct Node* newNode, struct Node* item);
void formDefaulterList(struct Node *root, FILE * f_ptr,  struct max_dept_defaulters * p);
void parse_rollno(char key[10], int *sem, int *dept_index, int * rollnumber );
void display_range(struct Node * head, int sem_search, int dept_index_search, int rollnumber1, int rollnumber2);
int compare (struct Node* node, char key[10]);
struct Node* minValueNode(struct Node* node);
int max(int a, int b);
int height(struct Node *N);
int getBalance(struct Node *N);
struct Node *leftRotate(struct Node *x);
struct Node *rightRotate(struct Node *y);




// each entry gives the subject index of MCC for a dept and for a sem,
// 5 dept (row wise), 4 semesters (column wise), MCC [0][0] - CS and 2 nd semester
int MCC[5][4]; 	
	
int max_attended [5][4]; // max_attended [0][0] - in CS for 2 nd semester, what is the max no of lectures attended for MCC
	
int main() 
{
	FILE * f_ptr; 
	FILE * f_eligible_ptr;
	FILE * f_less_attendance_ptr;
	FILE * f_defaulter_ptr;
	struct max_dept_defaulters p;

	int max_defaulter_dept_index, max_dep_defaulters;
	int read, line=0, k=0;
	
	char name[50];
	char dept[5];
	int sem;
	char rollno[10];
	
	char rollnumberstr[5];
	int attended[5];
	int conducted[5];
	char fee_status_str[10];
	char exam_applied[5];

	char delete_rollno[10];

 	struct Node *studentList = NULL;
	int i=0, j=0, sem_index, dept_index, max_conducted, mcc_sub;
	
	//initialize
	for(i=0; i<5; i++){
		for(j=0;j<4;j++){
			MCC[i][j] = -1;
			max_attended[i][j] = -1; 
		}
    }
	
	f_ptr = fopen("data.txt", "r");

	do{
		read = fscanf (f_ptr, "%d,%49[^,],%4[^,],%d,%9[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%9[^,],%s", &k, name, dept, &sem, rollno, 
		&conducted[0], &attended[0], &conducted[1], &attended[1], &conducted[2], &attended[2], &conducted[3], &attended[3], &conducted[4], &attended[4],	
		fee_status_str, exam_applied);
    
    	line++;
    	    	
    	if (read != 17 && !feof(f_ptr))
    	{
      		printf("\nline no = %d , File format incorrect.\n", line);    		
    	}
    
    	if (ferror(f_ptr))
    	{
      		printf("Error reading file.\n");
    	}
    	
    	struct Node *newNode = (struct Node*)malloc(sizeof(struct Node)); 
  		strcpy(newNode->name, name);
  		strcpy(newNode->dept, dept);
  		if(strcmp(newNode->dept, "CS") == 0)
    		newNode->dept_index =0;
    	else if(strcmp(newNode->dept, "CV") == 0)
    		newNode->dept_index =1;
    	else if(strcmp(newNode->dept, "EC") == 0)
    		newNode->dept_index =2;
		else if(strcmp(newNode->dept, "EE") == 0)
    		newNode->dept_index =3;
		else if(strcmp(newNode->dept, "ME") == 0)
    		newNode->dept_index =4;
    	
  		newNode->sem = sem;
  		strcpy(newNode->rollno, rollno);

    	for (k = 6, i=0; rollno[k] != '\0'; k++,i++) {
        	rollnumberstr[i] = rollno[k];
    	}
    	rollnumberstr[i] = '\0';   	
    	newNode->rollnumber = atoi(rollnumberstr);
    	
  		newNode->conducted[0] = conducted[0];
  		newNode->conducted[1] = conducted[1];
  		newNode->conducted[2] = conducted[2];
  		newNode->conducted[3] = conducted[3];
  		newNode->conducted[4] = conducted[4];
  		
  		newNode->attended[0] = attended[0];
  		newNode->attended[1] = attended[1];
  		newNode->attended[2] = attended[2];
		newNode->attended[3] = attended[3];
  		newNode->attended[4] = attended[4];
  		
  		newNode->mcc_attended = 0;
  		newNode->percent_attendance = 0;
  		
		strcpy(newNode->fee_status_str, fee_status_str);
		
		strcpy(newNode->exam_applied, exam_applied);
		
  		newNode->left = NULL;
  		newNode->right = NULL;
  		
  		sem_index =  ((newNode->sem) /2) -1;
    	dept_index = newNode->dept_index;
		
    	if(MCC[dept_index][sem_index]==-1)
    	{
    		max_conducted = newNode->conducted[0]; // max conducted lectures
    		mcc_sub = 0; // subject index
    		
    		for(i=1; i<5; i++)
			{
				if(newNode->conducted[i] > max_conducted)	
				{
					max_conducted = newNode->conducted[i];
					mcc_sub = i;
				}
			}
			MCC[dept_index][sem_index] = mcc_sub; 
			max_attended[dept_index][sem_index] = newNode->attended[mcc_sub];

		} 
		else
		{
			mcc_sub = MCC[dept_index][sem_index];
			if(newNode->attended[mcc_sub] > max_attended[dept_index][sem_index])
			{
				max_attended[dept_index][sem_index] = newNode->attended[mcc_sub];
			}
		}
		
    	studentList = insert(studentList, newNode);   
		
	} while (!feof(f_ptr) && (read==17));


   	fclose(f_ptr); 	

	updateMccPercentAttend(studentList); // this function call updates Mcc and percent attendance
	
	//struct Node* list_head= studentList;
  	//formList_applicant(list_head);
  	
  	int q_no;
  	char choice;
	do
	{
		printf("\nEnter question number = ");
		scanf("%d",&q_no);
	
		switch(q_no)
		{
			case 1:
				printf("\nQuestion 1\n");
				printf("General student record\n");
				display(studentList);
				
				break;
			case 2:
				printf("\nQuestion 2\n");
				printf("Students who did not apply for the examination\n");
				notAppliedExam(studentList);
				
				break;
			case 3:
				printf("Question 3\n");
				printf("List of all eligible students for the semester examination is stored in eligible.txt \n");
				f_eligible_ptr = fopen("eligible.txt", "w");
				formEligibleList(studentList,f_eligible_ptr);
				fclose(f_eligible_ptr);
			      
				break;
			case 4:
				printf("Question 4\n");
				printf("List of students having attendance <=75 is stored in less_attendance.txt \n");
				f_less_attendance_ptr = fopen("less_attendance.txt", "w");
				formLessThan75AttendanceList(studentList,f_less_attendance_ptr);
				fclose(f_less_attendance_ptr);
				
				break;
			case 5:
				printf("Question 5\n");
				printf(" To delete a student record, input roll number (For example BT19CS001) :");
				scanf("%s", delete_rollno);
				
				studentList = deleteNode(studentList, delete_rollno);
				
				break;
			case 6:
				printf("Question 6\n");
				printf("Students whose attendance is >75 for the respective MCC but their fee status is pending\n");

				feeStatusPendingAttend75(studentList);
				
				break;
			case 7:
				printf("Question 7\n");
				printf("List of defaulter students is stored in defaulter.txt \n");
				f_defaulter_ptr = fopen("defaulter.txt", "w");
				for(i=0;i<5;i++)
				{
					p.dept[i] = 0; //initialize
				}
				formDefaulterList(studentList,f_defaulter_ptr, &p);
				max_defaulter_dept_index = 0;
				max_dep_defaulters = p.dept[0];
				for(i=1;i<5;i++)
				{
					if(p.dept[i]> max_dep_defaulters)
					{
						max_dep_defaulters = p.dept[i];
						max_defaulter_dept_index = i;
					}
				}
				char default_dept[5];
				if ( max_defaulter_dept_index == 0 )
					strcpy ( default_dept, "CS" );					
				else if ( max_defaulter_dept_index == 1 )
					strcpy ( default_dept, "CV" );
				else if ( max_defaulter_dept_index == 2 )
					strcpy ( default_dept, "EC" );					
				else if ( max_defaulter_dept_index == 3 )
					strcpy ( default_dept, "EE" );						
				else if ( max_defaulter_dept_index == 4 )
					strcpy ( default_dept, "ME" );
					
				printf("Department with maximum defaulters is %s. count=%d", default_dept, max_dep_defaulters);

				fclose(f_defaulter_ptr);
				
				break;
			case 8:
				printf("Question 8 \n");
				printf("Range-search students based on the roll number. (BT22CS001 to BT22CS005)\n");
				char enrol1[10];
				char enrol2[10];
				int sem_search, dept_index_search, rollnumber1, rollnumber2;
				printf("\n Input Enrol num 1 : ");
				scanf("%s", enrol1);
				printf("\n Input Enrol num 2 : ");
				scanf("%s", enrol2);
				parse_rollno(enrol1, &sem_search, &dept_index_search, &rollnumber1);
				parse_rollno(enrol2, &sem_search, &dept_index_search, &rollnumber2);
				
				display_range(studentList, sem_search, dept_index_search, rollnumber1, rollnumber2);
				
				break;
			
			default:
				printf("No Choice Available \n");
				break;

		}
		
		printf("\n\nDo you wish to continue? (y/n) ");
		fflush(stdin);
		scanf("%c",&choice);
	}while(choice=='y'||choice=='Y');

}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Node *rightRotate(struct Node *y)
{
    struct Node *x = y->left;
    struct Node *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(height(y->left),
                    height(y->right)) + 1;
    x->height = max(height(x->left),
                    height(x->right)) + 1;
 
    // Return new root
    return x;
}
 
// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Node *leftRotate(struct Node *x)
{
    struct Node *y = x->right;
    struct Node *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(height(x->left),  
                    height(x->right)) + 1;
    y->height = max(height(y->left),
                    height(y->right)) + 1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of node N
int getBalance(struct Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
 
 // A utility function to get the height of the tree
int height(struct Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

/* UTILITY FUNCTIONS TO TEST isBalanced() FUNCTION */
 
/* returns maximum of two integers */
int max(int a, int b) { return (a >= b) ? a : b; }
 
/* Given a non-empty binary search
   tree, return the node
   with minimum key value found in
   that tree. Note that the
   entire tree does not need to be searched. */
struct Node* minValueNode(struct Node* node)
{
    struct Node* current = node;
 
    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL)
        current = current->left;
 
    return current;
}

void display_range(struct Node * head, int sem_search, int dept_index_search, int rollnumber1, int rollnumber2)
{   
    if (head != NULL) {
        display_range(head->left, sem_search, dept_index_search, rollnumber1, rollnumber2 );

		if( (head->sem == sem_search) && (head->dept_index = dept_index_search) && (head->rollnumber > rollnumber1 && head->rollnumber < rollnumber2))
		{		
    		printf("\nName = %s, Dept = %s, Sem = %d, Roll No = %s, Fee status = %s, Exam applied = %s, Percent Attendance = %d", head->name, head->dept, head->sem, 
 				head->rollno, head->fee_status_str, head->exam_applied, head->percent_attendance);
 		}
        display_range(head->right, sem_search, dept_index_search, rollnumber1, rollnumber2 );
    }
}

void parse_rollno(char key[10], int *sem, int *dept_index, int * rollnumber )
{
	int ret = 0, i,k;
	char dept_str[5];
	char rollno_str[5];
	char sem_str[5];
		
	// BT21CS001
	for (k = 6, i=0; key[k] != '\0'; k++,i++) {
        rollno_str[i] = key[k];
    }
    rollno_str[i] = '\0'; 
    *rollnumber = atoi(rollno_str);
	
	for (k = 4, i=0; k<6; k++,i++) {
        dept_str[i] = key[k];
    }
    dept_str[i] = '\0';	
    
  	if(strcmp(dept_str, "CS") == 0)
    	*dept_index =0;
    else if(strcmp(dept_str, "CV") == 0)
    	*dept_index =1;
    else if(strcmp(dept_str, "EC") == 0)
    	*dept_index =2;
	else if(strcmp(dept_str, "EE") == 0)
    	*dept_index =3;
	else if(strcmp(dept_str, "ME") == 0)
    	*dept_index =4;
    	
    for (k = 2, i=0; k<4; k++,i++) {
        sem_str[i] = key[k];
    }
    sem_str[i] = '\0';
    
    if(strcmp(sem_str, "19") == 0)
    	*sem =8;
    else if(strcmp(sem_str, "20") == 0)
    	*sem =6;
    else if(strcmp(sem_str, "21") == 0)
    	*sem =4;
	else if(strcmp(sem_str, "22") == 0)
    	*sem =2;
    	
    //printf("\n parse_rollno   sem=%d, dept_index =%d, rollnumber = %d", *sem, *dept_index, *rollnumber);
		
}

int compare (struct Node* node, char key[10])
{	
	int ret = 0, rollnumber, dept_index, sem;

	parse_rollno(key, &sem, &dept_index, &rollnumber);	
	
//	printf("\n NODE sem=%d, dept_index =%d, rollno = %d", node->sem, node->dept_index, node->rollnumber);
//	printf("\n KEY   sem=%d, dept_index =%d, rollno = %d", sem, dept_index, rollnumber);

    // Otherwise, recur down the tree
    if (sem < node->sem)
        ret = -1;
    else if (sem > node->sem)
        ret = 1;
    
    else
    {
    	if (dept_index < node->dept_index)
        	ret = -1;
    	else if (dept_index > node->dept_index)
        	ret = 1;
        else
        {
        	if (rollnumber < node->rollnumber)
        		ret = -1;
    		else if (rollnumber > node->rollnumber)
        		ret = 1;
		}
	}
//	printf("\n Compare....ret =%d", ret);
    return ret;
}

struct Node* deleteNode(struct Node* root, char key[10])
{
    // STEP 1: PERFORM STANDARD BST DELETE

    if (root == NULL)
        return root;
 
    // If the key to be deleted
    // is smaller than the root's
    // key, then it lies in left subtree
    if (compare(root, key) ==-1)
        root->left = deleteNode(root->left, key);
 
    // If the key to be deleted
    // is greater than the root's
    // key, then it lies in right subtree
    else if (compare(root, key) == 1)
        root->right = deleteNode(root->right, key);
 
    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left :
                                             root->right;
 
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of
                            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            struct Node* temp = minValueNode(root->right);
 
            // Copy the inorder
        	// successor's content to this node
        	copy(root,temp);
 
            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->rollno);
        }
    }
 
    // If the tree had only one node then return
    if (root == NULL)
      return root;
 
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));
 
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);
 
    // If this node becomes unbalanced, then there are 4 cases
 
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
 
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
 
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
 
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
 
    return root;
}

void updateMccPercentAttend(struct Node* temp)
{
	int sem_index, dept_index, mcc_sub;

    if (temp == NULL)
        return;
 
    /* first recur on left child */
    updateMccPercentAttend(temp->left);
 
    /* then print the data of node */
    sem_index =  ((temp->sem) /2) -1;
    dept_index = temp->dept_index;		
		
	mcc_sub = MCC[dept_index][sem_index]; // mcc_sub is the subject for which max classes are conducted
		
	temp->mcc_attended = temp->attended[mcc_sub]; // student attended these many lectures for MCC
		
	temp->percent_attendance = (100 * temp->mcc_attended ) / max_attended[dept_index][sem_index];
		
    //printf("%d ", node->data);
 
    /* now recur on right child */
    updateMccPercentAttend(temp->right);
}

void copy(struct Node* newNode, struct Node* item)
{
        
    strcpy(newNode->name, item->name);
  	strcpy(newNode->dept, item->dept);
  	newNode->dept_index = item->dept_index;
  	
  	newNode->sem = item->sem;
  	
  	strcpy(newNode->rollno, item->rollno);
  	newNode->rollnumber = item->rollnumber;
  	
  	newNode->conducted[0] = item->conducted[0];
  	newNode->conducted[1] = item->conducted[1];
  	newNode->conducted[2] = item->conducted[2];
  	newNode->conducted[3] = item->conducted[3];
  	newNode->conducted[4] = item->conducted[4];
  		
  	newNode->attended[0] = item->attended[0];
  	newNode->attended[1] = item->attended[1];
  	newNode->attended[2] = item->attended[2];
	newNode->attended[3] = item->attended[3];
  	newNode->attended[4] = item->attended[4];
  		
  	newNode->mcc_attended = item->mcc_attended;
  	newNode->percent_attendance = item->percent_attendance;
	strcpy(newNode->fee_status_str, item->fee_status_str);	
	strcpy(newNode->exam_applied, item->exam_applied);
}

struct Node* newNode(struct Node* item)
{
    struct Node* newNode
        = (struct Node*)malloc(sizeof(struct Node));
        
    strcpy(newNode->name, item->name);
    
  	strcpy(newNode->dept, item->dept);
  	newNode->dept_index = item->dept_index;
  	
  	newNode->sem = item->sem;
  	
  	strcpy(newNode->rollno, item->rollno);
  	newNode->rollnumber = item->rollnumber;
  	
  	newNode->conducted[0] = item->conducted[0];
  	newNode->conducted[1] = item->conducted[1];
  	newNode->conducted[2] = item->conducted[2];
  	newNode->conducted[3] = item->conducted[3];
  	newNode->conducted[4] = item->conducted[4];
  		
  	newNode->attended[0] = item->attended[0];
  	newNode->attended[1] = item->attended[1];
  	newNode->attended[2] = item->attended[2];
	newNode->attended[3] = item->attended[3];
  	newNode->attended[4] = item->attended[4];
  		
  	newNode->mcc_attended = 0;
  	newNode->percent_attendance = 0;
	strcpy(newNode->fee_status_str, item->fee_status_str);	
	strcpy(newNode->exam_applied, item->exam_applied);
    
    newNode->left = newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

void formDefaulterList(struct Node *root, FILE * f_ptr, struct max_dept_defaulters *p)
{   
    if (root != NULL) {
        formDefaulterList(root->left, f_ptr,p);
		if( (root->percent_attendance <= 75) || (strcmp(root->fee_status_str,"pending")==0) )
    	{
    		p->dept[root->dept_index] ++;   //pointer p contains array
    		     
    		fprintf(f_ptr, "\nName = %s, Dept = %s, Sem = %d, Roll No = %s, Fee status = %s, Exam applied = %s, Percent Attendance = %d", 
				root->name, root->dept, root->sem, root->rollno, root->fee_status_str, root->exam_applied, root->percent_attendance);
 		}
        formDefaulterList(root->right, f_ptr,p);
    }
}

void formLessThan75AttendanceList(struct Node *root, FILE * f_ptr)
{   
    if (root != NULL) {
        formLessThan75AttendanceList(root->left, f_ptr);
		if(root->percent_attendance < 75)
    	{
    		fprintf(f_ptr, "\nName = %s, Dept = %s, Sem = %d, Roll No = %s, Fee status = %s, Exam applied = %s, Percent Attendance = %d", 
				root->name, root->dept, root->sem, root->rollno, root->fee_status_str, root->exam_applied, root->percent_attendance);
 		}
        formLessThan75AttendanceList(root->right, f_ptr);
    }
}

void formEligibleList(struct Node *root, FILE * f_ptr)
{   
    if (root != NULL) {
        formEligibleList(root->left, f_ptr);
		if( (root->percent_attendance > 75) && (strcmp(root->fee_status_str,"clear")==0) && (strcmp(root->exam_applied,"A")==0) )
    	{
    		fprintf(f_ptr, "\nName = %s, Dept = %s, Sem = %d, Roll No = %s, Fee status = %s, Exam applied = %s, Percent Attendance = %d", 
				root->name, root->dept, root->sem, root->rollno, root->fee_status_str, root->exam_applied, root->percent_attendance);
 		}
        formEligibleList(root->right, f_ptr);
    }
}

struct Node* insert(struct Node* node, struct Node* key)
{
	/* 1.  Perform the normal BST insertion */
	
    // If the tree is empty, return a new node
    if (node == NULL)
        return newNode(key);
	
    // Otherwise, recur down the tree
    if (key->sem < node->sem)
        node->left = insert(node->left, key);
    else if (key->sem > node->sem)
        node->right = insert(node->right, key);
    //IF SEM IS EQUAL SORT BY DEPT
    else
    {
    	if (key->dept_index < node->dept_index)
        	node->left = insert(node->left, key);
    	else if (key->dept_index > node->dept_index)
        	node->right = insert(node->right, key);
        else
        {
        	if (key->rollnumber < node->rollnumber)
        		node->left = insert(node->left, key);
    		else if (key->rollnumber > node->rollnumber)
        		node->right = insert(node->right, key);
		}
	}
	
	/* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                        height(node->right));
 
    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);
 
    // If this node becomes unbalanced, then
    // there are 4 cases
 
    // Left Left Case
    if (balance > 1 && (compare(node->left, key->rollno) ==-1) )    // key < node->left->key) 
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && (compare(node->right, key->rollno) == 1) )   // key > node->right->key
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && (compare(node->left, key->rollno) == 1) ) // key > node->left->key
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Left Case
    if (balance < -1 && (compare(node->right, key->rollno) ==-1) ) // key < node->right->key
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    // Return the (unchanged) node pointer
    return node;
}

void display(struct Node *head)
{   
    if (head != NULL) {
        display(head->left);

    	printf("\nName = %s, Dept = %s, Sem = %d, Roll No = %s, Fee status = %s, Exam applied = %s, Percent Attendance = %d", head->name, head->dept, head->sem, 
 				head->rollno, head->fee_status_str, head->exam_applied, head->percent_attendance);
        display(head->right);
    }
}

void feeStatusPendingAttend75(struct Node* head)
{	
	if (head != NULL) 
	{
        feeStatusPendingAttend75(head->left);

		if( (strcmp(head->fee_status_str,"pending")==0) && ( head->percent_attendance > 75) )
        {
    		printf("\nName = %s, Dept = %s, Sem = %d, Roll No = %s, Fee status = %s, Exam applied = %s, Percent Attendance = %d", head->name, head->dept, head->sem, 
 				head->rollno, head->fee_status_str, head->exam_applied, head->percent_attendance);
 		}
        feeStatusPendingAttend75(head->right);
    }
}

void notAppliedExam(struct Node* head)
{	
	if (head != NULL) 
	{
        notAppliedExam(head->left);

		if( strcmp(head->exam_applied,"NA") == 0 )
        {
    		printf("\nName = %s, Dept = %s, Sem = %d, Roll No = %s, Fee status = %s, Exam applied = %s, Percent Attendance = %d", head->name, head->dept, head->sem, 
 				head->rollno, head->fee_status_str, head->exam_applied, head->percent_attendance);
 		}
        notAppliedExam(head->right);
    }
}





