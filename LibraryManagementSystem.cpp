#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// �ѥ����c��A�ΨӦs�x�ѥ����򥻫H��
typedef struct {
    char name[20];     // �ѥ��W��
    float price;       // �ѥ�����
    int quantity;      // �ѥ��ƶq�]�w�s�^
} Book;

// �`�I���c��A�ΨӺc�����
typedef struct Node {
    Book data;         // �ѥ��ƾ�
    struct Node* next; // ���V�U�@�Ӹ`�I�����w
} Node;

// �Ы�����Y�`�I
Node* createHeadNode() {
    Node* headNode = (Node*)malloc(sizeof(Node));
    if (!headNode) {
        perror("�L�k���t�Y�`�I�����s");
        exit(EXIT_FAILURE);
    }
    headNode->next = NULL; // ��l���Y�`�I�� next ���w����
    return headNode;
}

// �Ыؤ@�ӷs���`�I�A�æs�x�ѥ��H��
Node* createNode(Book data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("�L�k���t�s�`�I�����s");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;  // �N�ѥ��ƾڦs�J�s�`�I
    newNode->next = NULL;  // �N next ���w�]����
    return newNode;
}

// �ϥ��Y���k�N�s�ѥ����J���
void insertNodeByHead(Node* headNode, Book data) {
    Node* newNode = createNode(data); // �Ыطs�`�I
    newNode->next = headNode->next;   // �N�s�`�I�� next ���V�Y�`�I���U�@�Ӹ`�I
    headNode->next = newNode;         // �N�Y�`�I�� next ���V�s�`�I
}

// �ھڮѦW�R���`�I
void deleteNodeByName(Node* headNode, const char* bookName) {
    Node* previousNode = headNode;
    Node* currentNode = headNode->next;

    // �M�����d��ݭn�R�����ѥ��`�I
    while (currentNode != NULL && strcmp(currentNode->data.name, bookName) != 0) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (currentNode == NULL) {
        printf("\n>> �ѥ������\n\n");
        return;
    }

    // ����A�N�`�I�q������������񤺦s
    previousNode->next = currentNode->next;
    free(currentNode);
    printf("\n>> �ѥ��w�R��\n\n");
}

// ���L����Ҧ��ѥ��H��
void printBookList(Node* headNode) {
    Node* currentNode = headNode->next;
    if (!currentNode) {
        printf(">> �L���y�H��\n");
        return;
    }

    printf("�ѦW\t����\t�ƶq\n");
    while (currentNode) {
        printf("%s\t%.1f\t%d\n", currentNode->data.name, currentNode->data.price, currentNode->data.quantity);
        currentNode = currentNode->next;
    }
    printf("\n");
}

// �ϥΫ_�w�Ƨǫ������ѥ��C��i��Ƨ�
void bubbleSortBookList(Node* headNode) {
    if (headNode->next == NULL) return;

    Node* p;
    Node* q;

    // �_�w�ƧǺ�k
    for (p = headNode->next; p != NULL; p = p->next) {
        for (q = headNode->next; q->next != NULL; q = q->next) {
            if (q->data.price > q->next->data.price) {
                Book temp = q->data;
                q->data = q->next->data;
                q->next->data = temp;
            }
        }
    }

    // �Ƨǧ����ᥴ�L�ѥ��C��
    printBookList(headNode);
}

// �ھڮѦW�d��ѥ�
Node* searchByName(Node* headNode, const char* bookName) {
    Node* currentNode = headNode->next;
    while (currentNode != NULL && strcmp(currentNode->data.name, bookName) != 0) {
        currentNode = currentNode->next;
    }
    return currentNode;
}

// �O�s�ѥ��H������
void saveInfoToFile(const char* fileName, Node* headNode) {
    FILE* fp = fopen(fileName, "w");
    if (!fp) {
        perror("�L�k���}���i��g�J");
        return;
    }

    Node* currentNode = headNode->next;
    while (currentNode != NULL) {
        fprintf(fp, "%s\t%.1f\t%d\n", currentNode->data.name, currentNode->data.price, currentNode->data.quantity);
        currentNode = currentNode->next;
    }

    fclose(fp);
}

// �q���Ū���ѥ��H��
void readInfoFromFile(const char* fileName, Node* headNode) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf(">> ��󥼧��A�Ыطs���\n");
        fp = fopen(fileName, "w+");
        if (!fp) {
            perror("�L�k�Ыطs���");
            exit(EXIT_FAILURE);
        }
    }

    Book tempBook;
    while (fscanf(fp, "%s\t%f\t%d\n", tempBook.name, &tempBook.price, &tempBook.quantity) != EOF) {
        insertNodeByHead(headNode, tempBook);
    }

    fclose(fp);
}

// ��ܾާ@���
void displayMenu() {
    printf("----------------------\n");
    printf("�Ϯ��]�޲z�t��\n");
    printf("0. �h�X�t��\n");
    printf("1. �s�W�Ϯ�\n");
    printf("2. �s���{���Ϯ�\n");
    printf("3. �ɮ�\n");
    printf("4. �ٮ�\n");
    printf("5. �R���Ϯ�\n");
    printf("6. �d��Ϯ�\n");
    printf("7. �ƧǹϮ�\n"); 
    printf("----------------------\n");
    printf("�п�J�]0-7�^�G\n");
}

// �ھڥΤ��J�B�z�ާ@
void handleUserInput(Node* headNode) {
    int userChoice = 0;
    Book tempBook;
    Node* result = NULL;

    scanf("%d", &userChoice);

    switch (userChoice) {
    case 0:
        printf("\n>> �w�h�X�t��\n\n");
        exit(0);
        break;
    case 1:
        printf("�i �s�W�Ϯ� �j\n�п�J���y���W�١B����B�ƶq�G");
        scanf("%s%f%d", tempBook.name, &tempBook.price, &tempBook.quantity);
        insertNodeByHead(headNode, tempBook);
        saveInfoToFile("bookinfo.txt", headNode);
        printf("\n>> �s�W���\\n");
        break;
    case 2:
        printf("�i �s���{���Ϯ� �j\n");
        printBookList(headNode);
        break;
    case 3:
        printf("�i �ɮ� �j\n�п�J�ѦW�G");
        scanf("%s", tempBook.name);
        result = searchByName(headNode, tempBook.name);
        if (!result) {
            printf("\n>> �L���ѥ�\n\n");
        } else if (result->data.quantity > 0) {
            result->data.quantity--;
            printf("\n>> �ɮѦ��\\n");
        } else {
            printf("\n>> �ѥ��L�w�s\n\n");
        }
        break;
    case 4:
        printf("�i �ٮ� �j\n�п�J�ѦW�G");
        scanf("%s", tempBook.name);
        result = searchByName(headNode, tempBook.name);
        if (!result) {
            printf("\n>> ���Ѥ��ݩ󥻹Ϯ��]\n\n");
        } else {
            result->data.quantity++;
            printf("\n>> �ٮѦ��\\n\n");
        }
        break;
    case 5:
        printf("�i �R�����y �j\n�п�J�ѦW�G");
        scanf("%s", tempBook.name);
        deleteNodeByName(headNode, tempBook.name);
        saveInfoToFile("bookinfo.txt", headNode);
        break;
    case 6:
        printf("�i �d����y �j\n�п�J�ѦW�G");
        scanf("%s", tempBook.name);
        result = searchByName(headNode, tempBook.name);
        if (!result) {
            printf("\n>> �����ѥ�\n\n");
        } else {
            printf("\n>> �d�ߦ��\\n�ѦW\t����\t�ƶq\n");
            printf("%s\t%.1f\t%d\n\n", result->data.name, result->data.price, result->data.quantity);
        }
        break;
        printf("�i �п�J���ļƦr �j\n\n");
        break;
    case 7:
        printf("�i �ƧǹϮ� �j\n");
        bubbleSortBookList(headNode);
        break;
    default:
        printf("�i �п�J���ļƦr �j\n\n");
        break;
    }
}

// �D���
int main() {
    Node* bookList = createHeadNode(); // ��l�Ʈѥ����
    readInfoFromFile("bookinfo.txt", bookList); // �q���Ū���ƾ�

    while (1) {
        displayMenu();  // ��ܵ��
        handleUserInput(bookList);  // �B�z�Τ��J
    	system("pause");  // �Ȱ�����x���

		system("cls");  // �M������x���
	}

	system("cls");  // �{�������e�M��
	
    return 0;
}

