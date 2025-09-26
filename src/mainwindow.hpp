#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStyle>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QFileDialog>

#include "nodewidget.hpp"

#include "graph.hpp"
#include "analysis.hpp"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private:
  NodeWidget* nodeWidget_;

  QMenu* fileMenu_;
  QAction* importFileAction_;

  QToolBar* toolBar_;
  QAction* playAction_;
  QAction* pauseAction_;
  QAction* stopAction_;

  Graph* graph_;
  bool simulate_ = false;
  bool reset_ = false;

  void initMenuBar();
  void initToolBar();
  void connectSignals();

  QString getPath(const QString& caption, const QString& filter);

 private slots:
  void importFile();

  void playSimulation();
  void pauseSimulation();
  void stopSimulation();
};

#endif  // MAINWINDOW_H