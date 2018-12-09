#include "bbstree.h"

#include <cstdio>
#include <cstdlib>

//中序遍历
void InOrderTraverse(BBSTree T)
{
    if(NULL == T)return;
    InOrderTraverse(T->lchild);
    printf("%d ",T->data);
    InOrderTraverse(T->rchild);
}
//左旋转
void L_Rotation(BBSTree &T)
{
    BBSTree t;
    t = T->rchild;
    T->rchild = t->lchild;
    t->lchild = T;
    T = t;
}
//右旋转
void R_Rotation(BBSTree &T)
{
    BBSTree t;
    t = T->lchild;
    T->lchild = t->rchild;
    t->rchild = T;
    T = t;
}
//插入之后树T失衡时的平衡函数
//这里对三个节点进行定义
//ROOT=原来的树T
//RL=树T的左孩子
//RLR=树T的左孩子的右孩子
void L_Balance(BBSTree &T)
{
    BBSTree lc,rc;
    lc = T->lchild;
    //通过判断RL失衡的情况为LL还是LR
    switch(lc->bf)
    {
        //LL type
        //这种情况下平衡因子改变的节点只有ROOT和RL
        case LH:
        T->bf = lc->bf = EH;
        break;
        //LR type
        case RH://右子树高则说明插入的节点在RLR这棵树上
        rc = lc->rchild;
        //判断RLR的平衡因子
        //这种情况下平衡因子发生改变的节点为上述定义的三个节点
        switch(rc->bf)
        {
            //RLR的左子树高，旋转之后该左子树接到了RL的右子树上
            //由于RL一定不存在右子树，且左子树高度一定为1，所以RL的平衡因子为EH
            //ROOT右旋变成RLR的右子树，因此失去了左子树，并且由于RLR没有右子树，因此ROOT的变成右高
            case LH:T->bf=RH;lc->bf=EH;break;
            //ROOT这棵树只有三个节点的情况，包括ROOT，即ROOT的右子树为空，左子树高度为2
            case EH:T->bf=lc->bf=EH;break;
            //这种情况和上面左高的情况类似。
            case RH:T->bf=EH;lc->bf=LH;break;
        }
        rc->bf = EH;//通过调整之后RLR成为新的树根，左右子树等高
        //先左旋RL，将其转化成LL情况，再进行右旋
        L_Rotation(T->lchild);
        break;
    }
    R_Rotation(T);
}
//删除之后树T的左子树失衡时的平衡函数
//延用上面对三个节点的定义
Status L_D_Balance(BBSTree &T)
{
    BBSTree lc,rc;
    lc = T->lchild;
    Status result;
    switch(lc->bf)
    {
        //当RL的平衡因子不为等高的时候，其旋转后的新树高度会降低
        //需要通知其祖先节点继续进行失衡调整
        //这里平衡因子变化的情况和插入的失衡函数一样
        case LH://LL type
        T->bf = lc->bf = EH;
        result = YES;
        break;
        case RH://LR type
        rc = lc->rchild;
        switch(rc->bf)
        {
            case LH:T->bf=RH;lc->bf=EH;break;
            case EH:T->bf=lc->bf=EH;break;//only three node
            case RH:T->bf=EH;lc->bf=LH;break;
        }
        rc->bf = EH;
        L_Rotation(T->lchild);
        result = YES;
        break;
        //RL可能会出现该树的平衡因子为EH的情况
        //这种时候就要将其当成LL的情况处理，将树T进行右旋操作
        //并且右旋之后新树高度并没有改变
        case EH:
        T->bf=LH;lc->bf=RH;
        result = NO;
        break;
    }
    R_Rotation(T);
    return result;
}

//插入之后右子树失衡时的平衡函数
//和左子树的操作是对称相反的
void R_Balance(BBSTree &T)
{
    BBSTree lc,rc;
    rc = T->rchild;
    switch(rc->bf)
    {
        case RH:
        T->bf = rc->bf = EH;
        break;
        case LH:
        lc = rc->lchild;
        switch(lc->bf)
        {
            case RH:T->bf=LH;rc->bf=EH;break;
            case EH:T->bf=rc->bf=EH;break;
            case LH:T->bf=EH;rc->bf=RH;break;
        }
        lc->bf = EH;
        R_Rotation(T->rchild);
        break;
    }
    L_Rotation(T);
}
//删除之后右子树失衡时的平衡函数
//和左子树的操作是对称相反的
Status R_D_Balance(BBSTree &T)
{
    BBSTree lc,rc;
    rc = T->rchild;
    Status result;
    switch(rc->bf)
    {
        case RH:
        T->bf = rc->bf = EH;
        result =  YES;
        break;
        case LH:
        lc = rc->lchild;
        switch(lc->bf)
        {
            case RH:T->bf=LH;rc->bf=EH;break;
            case EH:T->bf=rc->bf=EH;break;
            case LH:T->bf=EH;rc->bf=RH;break;
        }
        lc->bf = EH;
        R_Rotation(T->rchild);
        result =  YES;
        break;
        case EH:
        T->bf=RH;rc->bf=LH;
        result = NO;
        break;
    }
    L_Rotation(T);
    return result;
}
//插入
Status InsertAVL(BBSTree &T,RcdType e,Status &taller)
{
    //如果为空则以该节点为根创建一棵新的树
    if(NULL==T)
    {
        T=(BBSTree)malloc(sizeof(BBSTNode));
        if(NULL==T)return NO;
        T->data=e;
        T->bf=EH;
        taller=YES;
        T->lchild=T->rchild=NULL;
    }
    //如果该节点已经存在，那么就直接返回
    else if (e==T->data)
    {
        taller = NO;
        return NO;
    }
    else if(e<T->data)
    {
        if(NO==InsertAVL(T->lchild,e,taller))return NO;
        if(YES==taller)//树变高了
        {
            switch(T->bf)
            {
                //本来左子树就高的话那么就失衡了
                case LH:L_Balance(T);taller=NO;break;
                case RH:T->bf=EH;taller=NO;break;
                case EH:T->bf=LH;taller=YES;break;
            }
        }
    }
    else
    {
        if(NO==InsertAVL(T->rchild,e,taller))return NO;
        if(YES==taller)
        {
            switch(T->bf)
            {
                case RH:R_Balance(T);taller=NO;break;
                case LH:T->bf=EH;taller=NO;break;
                case EH:T->bf=RH;taller=YES;break;
            }
        }
    }
    return YES;
}
//删除
Status DeleteAVL(BBSTree &T,RcdType e,Status &shorter)
{
    //找不到则返回
    if(NULL==T)
    {
        return NO;
    }
    else if (e<T->data)
    {
        if (NO==DeleteAVL(T->lchild,e,shorter))return NO;
        if(YES==shorter)
        {
            //判断祖先节点的高度
            switch(T->bf)
            {
                //如果本来左子树高的话，删除节点之后就应该变矮了
                case LH:T->bf=EH;shorter=YES;break;
                case EH:T->bf=RH;shorter=NO;break;
                case RH:shorter=R_D_Balance(T);break;
            }
        }
    }
    else if(e>T->data)
    {
        if (NO==DeleteAVL(T->rchild,e,shorter))return NO;
        if(YES==shorter)
        {
            switch(T->bf)
            {
                case RH:T->bf=EH;shorter=YES;break;
                case EH:T->bf=LH;shorter=NO;break;
                case LH:shorter=L_D_Balance(T);break;
            }
        }
    }
    else //找到要删除的节点
    {
        BBSTree p;
        p = T;
        //若左右子树都存在,那么寻找左子树的中序后继
        if(NULL!=T->lchild && NULL != T->rchild)
        {
                p = T->lchild;
                while (p->rchild != NULL) {
                    p = p->rchild;
                }
                //并将后继的值赋给该节点
                T->data = p->data;
                //接下来通过遍历该节点的左子树，找到后继删除
                DeleteAVL(T->lchild,p->data,shorter);
                if (shorter==YES)
                {
                    switch(T->bf)
                    {
                        case LH:T->bf=EH;shorter=YES;break;
                        case EH:T->bf=RH;shorter=NO;break;
                        case RH:shorter=R_D_Balance(T);break;
                    }
                }
        }
        else //选择左子树或者右子树接上
        {
            T = (NULL==T->lchild)?T->rchild:T->lchild;
            free(p);
            shorter = YES;
        }
    }
    return YES;
}

void DestroyBBST(BBSTree &T)//销毁树
{
    if(NULL==T)return;
    if(NULL!=T->lchild)DestroyBBST(T->lchild);
    if(NULL!=T->rchild)DestroyBBST(T->rchild);
    free(T);
    T=NULL;
}
BBSTree SearchBBST(BBSTree T,RcdType key)//搜索元素
{
    if (NULL==T)return NULL;
    if (T->data == key) return T;
    if (T->data > key) return SearchBBST(T->lchild,key);
    else return SearchBBST(T->rchild,key);
}


//返回两个数之间的最大值
int max(int a,int b)
{
    return a>b?a:b;
}
//求树的深度
int GetDepth(BBSTree T)
{
    if (NULL == T)return 0;
    return max(GetDepth(T->lchild),GetDepth(T->rchild))+1;
}
//检查一棵树是否为平衡二叉树
Status isBalanced(BBSTree T)
{
    if(NULL == T) return YES;
    if((T->lchild!= NULL && T->data<T->lchild->data)||(T->rchild!= NULL && T->data>T->rchild->data))return NO;
    if(NO == isBalanced(T->lchild)) return NO;
    if(NO == isBalanced(T->rchild)) return NO;
    int depthLeft = GetDepth(T->lchild);
    int depthRight = GetDepth(T->rchild);
    if (abs(depthLeft - depthRight) > 1)
        return NO;
    return YES;
}

//合并两颗平衡二叉树
void MergeBBSTree(BBSTree &T1,BBSTree T2)
{
    Status s;
    if(NULL == T2)return;
    if (NULL != T2->lchild) MergeBBSTree(T1,T2->lchild);
    InsertAVL(T1,T2->data,s);
    if (NULL != T2->rchild) MergeBBSTree(T1,T2->rchild);
    return;
}

void DivBBSTree(BBSTree R,BBSTree &T1,BBSTree &T2,RcdType e){
    if(NULL==R)return;
    DivBBSTree(R->lchild,T1,T2,e);
    Status s;
    if(R->data>e)InsertAVL(T2,R->data,s);
    else InsertAVL(T1,R->data,s);
    DivBBSTree(R->rchild,T1,T2,e);
}
