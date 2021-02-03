#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QPair>
#include <QDebug>
#include <QQueue>

int ** matrix;
int * visit;
int num;

bool direction = false;
bool weight = false;

QQueue<int> * queue;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::PrintMatrix()
{
    QString print;
    for(int i = 0; i < num; i++)
    {
        for(int j = 0; j < num; j++)
            print += QString::number(matrix[i][j]) + "    ";
        print += '\n';
    }
    ui->textEdit_DataStructure->setText(print);
}

void MainWindow::on_pushButton_Set_clicked()
{
    QString s = ui->lineEdit_Number->text();
    num = s.toInt();

    matrix = new int * [num];
    for(int i = 0; i < num; i++)
        matrix[i] = new int[num];
    for(int i = 0; i < num; i++)
        for(int j = 0; j < num; j++)
            matrix[i][j] = 0;
    visit = new int[num];

    if(ui->checkBox_Weight->isChecked())
        weight = true;
    else
        weight = false;
    if(ui->checkBox_Direction->isChecked())
        direction = true;
    else
        direction = false;

    if(weight==true&&direction==true)
    {
        ui->groupBox_Search->setEnabled(true);
        ui->groupBox_MST->setEnabled(false);
        ui->groupBox_ShortestPath->setEnabled(true);
    }
    else if(weight==true&&direction==false)
    {
        ui->groupBox_Search->setEnabled(true);
        ui->groupBox_MST->setEnabled(true);
        ui->groupBox_ShortestPath->setEnabled(true);
    }
    else if(weight==false&&direction==true)
    {
        ui->groupBox_Search->setEnabled(true);
        ui->groupBox_MST->setEnabled(false);
        ui->groupBox_ShortestPath->setEnabled(false);
    }
    else
    {
        ui->groupBox_Search->setEnabled(true);
        ui->groupBox_MST->setEnabled(false);
        ui->groupBox_ShortestPath->setEnabled(false);
    }

    PrintMatrix();
}

void MainWindow::on_pushButton_Link_clicked()
{
    int row = ui->lineEdit_Row->text().toInt();
    int col = ui->lineEdit_Column->text().toInt();
    int wgh = ui->lineEdit_Weight->text().toInt();
    if(weight==false)
        wgh = 1;

    if(direction==false)
    {
        matrix[row][col] = wgh;
        matrix[col][row] = wgh;
    }
    else
        matrix[row][col] = wgh;

    PrintMatrix();
}

void MainWindow::DFS(int start)
{
    visit[start] = 1;
    for(int i = 0; i < num; i++)
    {
        if(matrix[start][i] != 0 && !visit[i])
        {
            ui->textEdit_Result->append("Moved! : " + QString::number(start) + " to " + QString::number(i));
            DFS(i);
        }
    }
}

void MainWindow::BFS(int start)
{
    visit[start] = 1;
    queue->enqueue(start);
    while(!queue->isEmpty())
    {
        start = queue->dequeue();
        for(int i = 0; i < num; i++)
        {
            if(matrix[start][i]!=0&&!visit[i])
            {
                visit[i] = 1;
                ui->textEdit_Result->append("Moved! : " + QString::number(start) + " to " + QString::number(i));
                queue->enqueue(i);
            }
        }
    }
}

void MainWindow::Kruskal()
{
    int edgCount = 0;
    int indexE = 0;
    for(int i = 0; i < num; i++)
    {
        for(int j = i; j < num; j++)
        {
            if(matrix[i][j]!=0)
                edgCount++;
        }
    }
    int ** E = new int * [edgCount];
    for(int i = 0; i < edgCount; i++)
    {
        E[i] = new int[3];
    }
    for(int i = 0; i < num; i++)
    {
        for(int j = i; j < num; j++)
        {
            if(matrix[i][j]!=0)
            {
                E[indexE][0] = i;
                E[indexE][1] = j;
                E[indexE][2] = matrix[i][j];
                indexE++;
            }
        }
    }
    int * tmp;
    for(int i = 0; i < edgCount; i++)
    {
        for(int j = 0; j < edgCount-(i+1); j++)
        {
            if(E[j][2] > E[j+1][2])
            {
                tmp = E[j+1];
                E[j+1] = E[j];
                E[j] = tmp;
            }
        }
    }
    int * s = new int[num];
    for(int i = 0; i < num; i++)
        s[i] = i;
    int ** t = new int * [num-1];
    for(int i = 0; i < num-1; i++)
        t[i] = new int[2];

    edgCount = 0;
    int index = 0;
    int v1, v2;
    int s1, s2;

    while(edgCount < num-1)
    {
        v1 = E[index][0];
        v2 = E[index][1];
        s1 = s[v1];
        s2 = s[v2];
        if(s1 != s2)
        {
            for(int i = 0; i < num; i++)
                if(s[i] == s2)
                    s[i] = s1;
            t[edgCount][0] = v1;
            t[edgCount][1] = v2;
            edgCount++;
        }
        index++;
    }
    for(int i = 0; i < num-1; i++)
        ui->textEdit_Result->append("Select! Edge : Node" + QString::number(t[i][0]) + "-Node" + QString::number(t[i][1]));
}

void MainWindow::Prim()
{
    for(int i = 0; i < num; i++)
        for(int j = 0; j < num; j++)
            if(matrix[i][j]==0)
                matrix[i][j]=999;
    int ** t = new int * [num-1];
    for(int i = 0; i < num-1; i++)
        t[i] = new int [2];
    int * from = new int [num];
    int * dist = new int [num];

    for(int i = 0; i < num; i++)
    {
        from[i] = 0;
        dist[i] = matrix[0][i];
    }
    for(int i = 0; i < num-1; i++)
    {
        int best = 0;
        for(int j = 0; j < num; j++)
            if(dist[j]!=0 && dist[j]<dist[best])
                best = j;
        t[i][0] = from[best];
        t[i][1] = best;
        dist[best] = 0;

        for(int j = 1; j < num; j++)
            if(matrix[best][j] < dist[j])
            {
                from[j] = best;
                dist[j] = matrix[best][j];
            }
    }
    for(int i = 0; i < num-1; i++)
        ui->textEdit_Result->append("Select! Edge : Node" + QString::number(t[i][0]) + "-Node" + QString::number(t[i][1]));
    for(int i = 0; i < num; i++)
        for(int j = 0; j < num; j++)
            if(matrix[i][j]==999)
                matrix[i][j]=0;

}

void MainWindow::Sollin(){}

void MainWindow::Dijkstra()
{
    int ** t = new int * [num-1];
    for(int i = 0; i < num-1; i++)
        t[i] = new int[2];
    for(int i = 0; i < num; i++)
        for(int j = 0; j < num; j++)
            if(matrix[i][j]==0)
                matrix[i][j]=999;
    int * from = new int[num];
    int * dist = new int[num];
    for(int i = 0; i < num; i++)
    {
        from[i] = 0;
        dist[i] = matrix[0][i];
    }
    int best = 0;
    for(int i = 0; i < num-1; i++)
    {
        for(int j = 0; j < num; j++)
        {
            if(dist[j] != 0 && dist[best])
            {
                best = j;
            }
        }
        t[i][0] = from[best];
        t[i][1] = best;
        for(int k = 0; k < num; k++)
        {
            if(dist[best] + matrix[best][k] < dist[k])
            {
                from[k] = best;
                dist[k] = dist[best] + matrix[best][k];
            }
        }
        dist[best] = 0;
    }
    for(int i = 0; i < num-1; i++)
        ui->textEdit_Result->append("Select! Edge : Node" + QString::number(t[i][0]) + "-Node" + QString::number(t[i][1]));
    for(int i = 0; i < num; i++)
        for(int j = 0; j < num; j++)
            if(matrix[i][j]==999)
                matrix[i][j]=0;

}

void MainWindow::Floyd()
{
    for(int i = 0; i < num; i++)
        for(int j = 0; j < num; j++)
            if(matrix[i][j]==0)
                matrix[i][j]=999;
    int ** d = new int * [num];
    int ** p = new int * [num];
    for(int i = 0; i < num; i++)
    {
        d[i] = new int[num];
        p[i] = new int[num];
    }
    for(int i = 0; i < num; i++)
        for(int j = 0; j < num; j++)
            d[i][j] = matrix[i][j];
    for(int a = 0; a < num; a++)
        for(int i = 0; i < num; i++)
            for(int j = 0; j < num; j++)
                if(d[i][j] > d[i][a]+d[a][j])
                {
                    d[i][j] = d[i][a] + d[a][j];
                    p[i][j] = a;
                }
    for(int i = 0; i < num; i++)
        for(int j = 0; j < num; j++)
            if(matrix[i][j]==999)
                matrix[i][j]=0;

    QString print;
    for(int i = 0; i < num; i++)
    {
        for(int j = 0; j < num; j++)
            print += QString::number(d[i][j]) + "    ";
        print += '\n';
    }
    ui->textEdit_Result->setText(print);
}
void MainWindow::on_pushButton_Search_clicked()
{
    ui->textEdit_Result->setText("");
    int start = ui->lineEdit->text().toInt();
    if(ui->radioButton_DFS->isChecked())
    {
        for(int i = 0; i < num; i++)
            visit[i] = 0;
        DFS(start);
    }
    else if(ui->radioButton_BFS->isChecked())
    {
        for(int i = 0; i < num; i++)
            visit[i] = 0;
        queue = new QQueue<int>();
        BFS(start);
    }
}

void MainWindow::on_pushButton_ShortestPath_clicked()
{
    ui->textEdit_Result->setText("");
    if(ui->radioButton_Dijkstra->isChecked())
        Dijkstra();
    else if(ui->radioButton_FloydWarshall->isChecked())
        Floyd();
    //else if(ui->radioButton_Prim->isChecked())
}

void MainWindow::on_pushButton_MST_clicked()
{
    ui->textEdit_Result->setText("");
    if(ui->radioButton_Kruskal->isChecked())
        Kruskal();
    else if(ui->radioButton_Prim->isChecked())
        Prim();

}
