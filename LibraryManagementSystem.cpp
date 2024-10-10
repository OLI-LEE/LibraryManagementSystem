#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 書本結構體，用來存儲書本的基本信息
typedef struct {
    char name[20];     // 書本名稱
    float price;       // 書本價格
    int quantity;      // 書本數量（庫存）
} Book;

// 節點結構體，用來構建鏈表
typedef struct Node {
    Book data;         // 書本數據
    struct Node* next; // 指向下一個節點的指針
} Node;

// 創建鏈表的頭節點
Node* createHeadNode() {
    Node* headNode = (Node*)malloc(sizeof(Node));
    if (!headNode) {
        perror("無法分配頭節點的內存");
        exit(EXIT_FAILURE);
    }
    headNode->next = NULL; // 初始化頭節點的 next 指針為空
    return headNode;
}

// 創建一個新的節點，並存儲書本信息
Node* createNode(Book data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("無法分配新節點的內存");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;  // 將書本數據存入新節點
    newNode->next = NULL;  // 將 next 指針設為空
    return newNode;
}

// 使用頭插法將新書本插入鏈表
void insertNodeByHead(Node* headNode, Book data) {
    Node* newNode = createNode(data); // 創建新節點
    newNode->next = headNode->next;   // 將新節點的 next 指向頭節點的下一個節點
    headNode->next = newNode;         // 將頭節點的 next 指向新節點
}

// 根據書名刪除節點
void deleteNodeByName(Node* headNode, const char* bookName) {
    Node* previousNode = headNode;
    Node* currentNode = headNode->next;

    // 遍歷鏈表查找需要刪除的書本節點
    while (currentNode != NULL && strcmp(currentNode->data.name, bookName) != 0) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (currentNode == NULL) {
        printf("\n>> 書本未找到\n\n");
        return;
    }

    // 找到後，將節點從鏈表中移除並釋放內存
    previousNode->next = currentNode->next;
    free(currentNode);
    printf("\n>> 書本已刪除\n\n");
}

// 打印鏈表中所有書本信息
void printBookList(Node* headNode) {
    Node* currentNode = headNode->next;
    if (!currentNode) {
        printf(">> 無書籍信息\n");
        return;
    }

    printf("書名\t價格\t數量\n");
    while (currentNode) {
        printf("%s\t%.1f\t%d\n", currentNode->data.name, currentNode->data.price, currentNode->data.quantity);
        currentNode = currentNode->next;
    }
    printf("\n");
}

// 使用冒泡排序按價格對書本列表進行排序
void bubbleSortBookList(Node* headNode) {
    if (headNode->next == NULL) return;

    Node* p;
    Node* q;

    // 冒泡排序算法
    for (p = headNode->next; p != NULL; p = p->next) {
        for (q = headNode->next; q->next != NULL; q = q->next) {
            if (q->data.price > q->next->data.price) {
                Book temp = q->data;
                q->data = q->next->data;
                q->next->data = temp;
            }
        }
    }

    // 排序完成後打印書本列表
    printBookList(headNode);
}

// 根據書名查找書本
Node* searchByName(Node* headNode, const char* bookName) {
    Node* currentNode = headNode->next;
    while (currentNode != NULL && strcmp(currentNode->data.name, bookName) != 0) {
        currentNode = currentNode->next;
    }
    return currentNode;
}

// 保存書本信息到文件
void saveInfoToFile(const char* fileName, Node* headNode) {
    FILE* fp = fopen(fileName, "w");
    if (!fp) {
        perror("無法打開文件進行寫入");
        return;
    }

    Node* currentNode = headNode->next;
    while (currentNode != NULL) {
        fprintf(fp, "%s\t%.1f\t%d\n", currentNode->data.name, currentNode->data.price, currentNode->data.quantity);
        currentNode = currentNode->next;
    }

    fclose(fp);
}

// 從文件讀取書本信息
void readInfoFromFile(const char* fileName, Node* headNode) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf(">> 文件未找到，創建新文件\n");
        fp = fopen(fileName, "w+");
        if (!fp) {
            perror("無法創建新文件");
            exit(EXIT_FAILURE);
        }
    }

    Book tempBook;
    while (fscanf(fp, "%s\t%f\t%d\n", tempBook.name, &tempBook.price, &tempBook.quantity) != EOF) {
        insertNodeByHead(headNode, tempBook);
    }

    fclose(fp);
}

// 顯示操作菜單
void displayMenu() {
    printf("----------------------\n");
    printf("圖書館管理系統\n");
    printf("0. 退出系統\n");
    printf("1. 新增圖書\n");
    printf("2. 瀏覽現有圖書\n");
    printf("3. 借書\n");
    printf("4. 還書\n");
    printf("5. 刪除圖書\n");
    printf("6. 查找圖書\n");
    printf("7. 排序圖書\n"); 
    printf("----------------------\n");
    printf("請輸入（0-7）：\n");
}

// 根據用戶輸入處理操作
void handleUserInput(Node* headNode) {
    int userChoice = 0;
    Book tempBook;
    Node* result = NULL;

    scanf("%d", &userChoice);

    switch (userChoice) {
    case 0:
        printf("\n>> 已退出系統\n\n");
        exit(0);
        break;
    case 1:
        printf("【 新增圖書 】\n請輸入書籍的名稱、價格、數量：");
        scanf("%s%f%d", tempBook.name, &tempBook.price, &tempBook.quantity);
        insertNodeByHead(headNode, tempBook);
        saveInfoToFile("bookinfo.txt", headNode);
        printf("\n>> 新增成功\n");
        break;
    case 2:
        printf("【 瀏覽現有圖書 】\n");
        printBookList(headNode);
        break;
    case 3:
        printf("【 借書 】\n請輸入書名：");
        scanf("%s", tempBook.name);
        result = searchByName(headNode, tempBook.name);
        if (!result) {
            printf("\n>> 無此書本\n\n");
        } else if (result->data.quantity > 0) {
            result->data.quantity--;
            printf("\n>> 借書成功\n");
        } else {
            printf("\n>> 書本無庫存\n\n");
        }
        break;
    case 4:
        printf("【 還書 】\n請輸入書名：");
        scanf("%s", tempBook.name);
        result = searchByName(headNode, tempBook.name);
        if (!result) {
            printf("\n>> 此書不屬於本圖書館\n\n");
        } else {
            result->data.quantity++;
            printf("\n>> 還書成功\n\n");
        }
        break;
    case 5:
        printf("【 刪除書籍 】\n請輸入書名：");
        scanf("%s", tempBook.name);
        deleteNodeByName(headNode, tempBook.name);
        saveInfoToFile("bookinfo.txt", headNode);
        break;
    case 6:
        printf("【 查找書籍 】\n請輸入書名：");
        scanf("%s", tempBook.name);
        result = searchByName(headNode, tempBook.name);
        if (!result) {
            printf("\n>> 未找到書本\n\n");
        } else {
            printf("\n>> 查詢成功\n書名\t價格\t數量\n");
            printf("%s\t%.1f\t%d\n\n", result->data.name, result->data.price, result->data.quantity);
        }
        break;
        printf("【 請輸入有效數字 】\n\n");
        break;
    case 7:
        printf("【 排序圖書 】\n");
        bubbleSortBookList(headNode);
        break;
    default:
        printf("【 請輸入有效數字 】\n\n");
        break;
    }
}

// 主函數
int main() {
    Node* bookList = createHeadNode(); // 初始化書本鏈表
    readInfoFromFile("bookinfo.txt", bookList); // 從文件中讀取數據

    while (1) {
        displayMenu();  // 顯示菜單
        handleUserInput(bookList);  // 處理用戶輸入
    	system("pause");  // 暫停控制台顯示

		system("cls");  // 清除控制台顯示
	}

	system("cls");  // 程式結束前清屏
	
    return 0;
}

