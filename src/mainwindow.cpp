#include "mainwindow.hpp"

#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      nodeWidget_(new NodeWidget(this)),
      fileMenu_(new QMenu(this)),
      importFileAction_(new QAction(this)),
      toolBar_(new QToolBar(this)),
      playAction_(new QAction(this)),
      pauseAction_(new QAction(this)),
      stopAction_(new QAction(this)),
      graph_(new Graph()) {
  setWindowTitle("Network Simulator 2");

  setMinimumSize(700, 500);

  setCentralWidget(nodeWidget_);

  initMenuBar();
  initToolBar();
  connectSignals();
}

MainWindow::~MainWindow() {
  delete nodeWidget_;

  delete fileMenu_;
  delete importFileAction_;

  delete toolBar_;
  delete playAction_;
  delete pauseAction_;
  delete stopAction_;

  delete graph_;
}

/**
 * @brief Initializes the menubar
 *
 * Sets up the hierarchy for the menus and respective actions.
 * Also sets the texts for the different menus and actions.
 */
void MainWindow::initMenuBar() {
  QMenuBar* bar = menuBar();

  fileMenu_->setTitle("&File");

  importFileAction_->setText("Import &file");
  fileMenu_->addAction(importFileAction_);

  bar->addMenu(fileMenu_);
}

/**
 * @brief Initializes the toolbar
 *
 * Sets up the start, pause, and stop simulation actions with
 * respective tooltips.
 */
void MainWindow::initToolBar() {
  playAction_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
  playAction_->setToolTip("Start simulation");
  toolBar_->addAction(playAction_);
  pauseAction_->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
  pauseAction_->setToolTip("Pause simulation");
  toolBar_->addAction(pauseAction_);
  stopAction_->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
  stopAction_->setToolTip("Stop simulation");
  toolBar_->addAction(stopAction_);

  addToolBar(toolBar_);
}

/**
 * @brief Gets user selected path
 *
 * Shows modal file dialog to user for selecting a specific file
 * matching given filter.
 *
 * @param caption: The caption for the file dialog.
 * @param filter: The filter for selecting file. Only files matching filter may
 * be selected.
 * @return Path to the selected file.
 */
QString MainWindow::getPath(const QString& caption, const QString& filter) {
  return QFileDialog::getOpenFileName(this, caption, "/home", filter);
}

/**
 * @brief Connects signals to slots
 *
 * Connects all needed signals to respective slots.
 */
void MainWindow::connectSignals() {
  // File actions
  connect(importFileAction_, &QAction::triggered, this,
          &MainWindow::importFile);

  // Toolbar
  connect(playAction_, &QAction::triggered, this, &MainWindow::playSimulation);
  connect(pauseAction_, &QAction::triggered, this,
          &MainWindow::pauseSimulation);
  connect(stopAction_, &QAction::triggered, this, &MainWindow::stopSimulation);
}

/**
 * @brief Imports nodes from file
 */
void MainWindow::importFile() {
  QString path = getPath("Import file", "Text file (*.txt)");

  if (path.isNull()) {  // If user pressed cancel -> path.isNull() == true
    return;
  }

  graph_->ReadGraph(path.toStdString());
}

/**
 * @brief Plays simulation
 *
 * Starts the simulation if it has not been started yet.
 * If paused it resumes the simulation.
 */
void MainWindow::playSimulation() {
  simulate_ = true;

  if (reset_) {
    nodeWidget_->reset();
    reset_ = false;
  }

  while (simulate_) {
    // Simulate one disscrete step
    graph_->TickIncrease();

    // Update nodewidget
    nodeWidget_->updateValues(graph_);

    // Process gui events to prevent blocking of gui
    // (not the best way to do this, eg threads would be much better)
    qApp->processEvents();
  }
}

/**
 * @brief Pauses simulation
 */
void MainWindow::pauseSimulation() { simulate_ = false; }

/**
 * @brief Stops simulation
 *
 * Stops simulation altogether and cleans up after the simulation.
 */
void MainWindow::stopSimulation() {
  simulate_ = false;
  reset_ = true;
  delete graph_;
  graph_ = new Graph();
}