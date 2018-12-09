/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "graphwidget.h"
#include "edge.h"
#include "node.h"

#include <math.h>
#include <QInputDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QSpinBox>
#include <QRandomGenerator>
#include <QDialogButtonBox>
#include <QFormLayout>

//! [0]
GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    scene->setSceneRect(0, 0,400,400);
    setMinimumSize(400, 400);
    setWindowTitle(tr("Elastic Nodes"));
//! [0]

//! [1]  
//    Status s;
//    for (int i=0;i<30;++i){
//        InsertAVL(t,i,s);
//    }
//    centerNode = new Node(this,t);
//    int depth = GetDepth(t);
//    qreal x = 20;
//    paintTree(centerNode,x,20,scene);
//    scene->setSceneRect(0, 0,x-20, 40*depth);
}
//! [1]


void GraphWidget::addNode(){
    bool flag = false;
    int number = 0;
    if(trees.size()>1){
        number = QInputDialog::getInt(NULL,tr("选择树"),tr("请输入树的编号(从左到右)"),std::rand()%trees.size(),0,trees.size()-1,2,&flag);
        if(!flag){
            //QMessageBox(tr("请规范输入")).show();
            return;
        }
    }
    int input = QInputDialog::getInt(NULL,tr("插入数字"),tr("请输入0-99的数字"),std::rand()%100,0,100,2,&flag);
    if(!flag){
        //QMessageBox(tr("请规范输入")).show();
        return;
    }
    if(trees.empty()){
      //  trees.insert(0,NULL);
        trees.push_back(NULL);
    }
    Status s;
    if(InsertAVL(trees.at(number),input,s) == NO){
        QMessageBox::about(NULL,tr("提醒"),tr("该数字已经存在"));
        return;
    }
    drawTree();
}

void GraphWidget::deleteNode(){
    int number = 0;
    bool flag = false;
    if(trees.size()>1){
        number = QInputDialog::getInt(NULL,tr("选择树"),tr("请输入树的编号(从左到右)"),std::rand()%trees.size(),0,trees.size()-1,2,&flag);
        if(!flag){
            //QMessageBox(tr("请规范输入")).show();
            return;
        }
    }
    int input = QInputDialog::getInt(NULL,tr("删除数字"),tr("请输入0-99的数字"),std::rand()%100,0,100,2,&flag);
    if(!flag){
        return;
    }
    Status s;
    if(DeleteAVL(trees.at(number),input,s) == NO){
        QMessageBox::about(NULL,tr("提醒"),tr("该数字不存在"));
        return;
    }
    drawTree();
}

void GraphWidget::searchNode(){
    int number = 0;
    bool flag = false;
    if(trees.size()>1){
        number = QInputDialog::getInt(NULL,tr("选择树"),tr("请输入树的编号(从左到右)"),std::rand()%trees.size(),0,trees.size()-1,2,&flag);
        if(!flag){
            return;
        }
    }
    int input = QInputDialog::getInt(NULL,tr("搜索数字"),tr("请输入0-99的数字"),std::rand()%100,0,100,2,&flag);
    if(!flag){
        return;
    }
    BBSTree s = SearchBBST(trees.at(number),input);
    if(s == NULL){
        QMessageBox::about(NULL,tr("提醒"),tr("该数字不存在"));
        return;
    }
    //更新点
    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
        {
            node->isSearched=node->node->data==s->data;
            node->update();
        }
    }
}

void GraphWidget::divTree(){
    int number = 0;
    bool flag = false;
    if(trees.size()>1){
        number = QInputDialog::getInt(NULL,tr("选择树"),tr("请输入树的编号(从左到右)"),std::rand()%trees.size(),0,trees.size()-1,2,&flag);
        if(!flag){
            //QMessageBox(tr("请规范输入")).show();
            return;
        }
    }
    int input = QInputDialog::getInt(NULL,tr("中间数"),tr("请输入0-99的数字"),std::rand()%100,0,100,2,&flag);
    if(!flag){
        return;
    }
    BBSTree temp1=NULL,temp2=NULL;
    DivBBSTree(trees.at(number),temp1,temp2,input);
    trees[number]=temp1;
    trees.push_back(temp2);
    drawTree();
}

void GraphWidget::mergerTree(){
    QDialog dialog(this);
    QFormLayout form(&dialog);
    //form.addRow(QLabel(tr("选择要合并的两颗树:")));
    // Value1
    QString value1 = QString("树1: ");
    QSpinBox *spinbox1 = new QSpinBox(&dialog);
    spinbox1->setRange(0, trees.size()-1);
    form.addRow(value1, spinbox1);
    // Value2
    QString value2 = ("树2: ");
    QSpinBox *spinbox2 = new QSpinBox(&dialog);
    form.addRow(value2, spinbox2);
    spinbox2->setRange(0, trees.size()-1);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    if (dialog.exec() == QDialog::Accepted) {
        int t1 = spinbox1->value();
        int t2 = spinbox2->value();
        if(t1==t2){
            QMessageBox::about(NULL,tr("提醒"),tr("请选择不同的两颗树"));
            return;
        }
        MergeBBSTree(trees[t1],trees[t2]);
        std::vector<BBSTree>::iterator it = trees.begin()+t2;
        trees.erase(it);
        drawTree();
    }
}

//! [2]
void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}
//! [2]
void GraphWidget::EmptyNodeAndEdge(){
    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            scene()->removeItem(node);
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
            scene()->removeItem(edge);
    }
}

//绘制二叉树
void GraphWidget::drawTree(){
    EmptyNodeAndEdge();
    qreal x = 20;
    int depth = 0;
    foreach(BBSTree t,trees){
        int t_d = GetDepth(t);
        depth=depth<t_d?t_d:depth;
        Node *n = new Node(this,t);
        paintTree(n,x,20,scene());
    }
    scene()->setSceneRect(0, 0,x-20, 40*depth);
}

void GraphWidget::paintTree(Node* &root,qreal &centerX,qreal centerY,QGraphicsScene *scene){
    if(root->node==NULL)return;
    if(root->node->lchild!=NULL){
        Node *lnode = new Node(this,root->node->lchild);
        scene->addItem(new Edge(root,lnode));
        paintTree(lnode,centerX,centerY+40,scene);
    }
    scene->addItem(root);
    root->setPos(centerX,centerY);
    centerX+=40;
    if(root->node->rchild!=NULL){
        Node *rnode = new Node(this,root->node->rchild);
        scene->addItem(new Edge(root,rnode));
        paintTree(rnode,centerX,centerY+40,scene);
    }
}

//! [3]
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
//    case Qt::Key_Up:
//        centerNode->moveBy(0, -20);
//        break;
//    case Qt::Key_Down:
//        centerNode->moveBy(0, 20);
//        break;
//    case Qt::Key_Left:
//        centerNode->moveBy(-20, 0);
//        break;
//    case Qt::Key_Right:
//        centerNode->moveBy(20, 0);
//        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
//! [3]

//! [4]
void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    return;

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}
//! [4]

#if QT_CONFIG(wheelevent)
//! [5]
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
//! [5]
#endif

//! [6]
void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
//    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
//                    sceneRect.width() - 4, sceneRect.height() - 4);
//    QString message(tr("Click and drag the nodes around, and zoom with the mouse "
//                       "wheel or the '+' and '-' keys"));

//    QFont font = painter->font();
//    font.setBold(true);
//    font.setPointSize(14);
//    painter->setFont(font);
//    painter->setPen(Qt::lightGray);
//    painter->drawText(textRect.translated(2, 2), message);
//    painter->setPen(Qt::black);
//    painter->drawText(textRect, message);
}
//! [6]

//! [7]
void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
//! [7]

void GraphWidget::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + QRandomGenerator::global()->bounded(300), -150 + QRandomGenerator::global()->bounded(300));
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
