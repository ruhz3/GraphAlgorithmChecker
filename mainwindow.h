#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void PrintMatrix();
    void on_pushButton_Set_clicked();
    void on_pushButton_Link_clicked();
    void on_pushButton_Search_clicked();
    void DFS(int start);
    void BFS(int start);
    void Kruskal();
    void Prim();
    void Sollin();
    void Dijkstra();
    void Floyd();
    void on_pushButton_ShortestPath_clicked();

    void on_pushButton_MST_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

