#ifndef BBSTREE_H
#define BBSTREE_H

#define LH +1
#define EH 0
#define RH -1


enum Status
{
    OK,ERROR,YES,NO
};

typedef int RcdType;
typedef struct BBSTNode{
    RcdType data;
    int bf;//平衡因子
    struct BBSTNode *lchild,*rchild;
} * BBSTree;
void InOrderTraverse(BBSTree T);
void L_Rotation(BBSTree &T);
void R_Rotation(BBSTree &T);
void L_Balance(BBSTree &T);
void R_Balance(BBSTree &T);
Status L_D_Balance(BBSTree &T);
Status R_D_Balance(BBSTree &T);

Status InsertAVL(BBSTree &T,RcdType e,Status &taller);
Status DeleteAVL(BBSTree &T,RcdType e,Status &shorter);

void DestroyBBST(BBSTree &T);
BBSTree SearchBBST(BBSTree T,RcdType key);

Status isBalanced(BBSTree T);

int GetDepth(BBSTree T);

void MergeBBSTree(BBSTree &T1,BBSTree T2);

void DivBBSTree(BBSTree R,BBSTree &T1,BBSTree &T2,RcdType e);

#endif // BBSTREE_H
