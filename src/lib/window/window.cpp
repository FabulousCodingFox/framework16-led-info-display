#include "./window.hpp"
#include "../usb/usb.hpp"
#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSlider>
#include <QString>
#include <QSystemTrayIcon>
#include <QTabWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

namespace lib::window
{
  SettingsTab::SettingsTab(QWidget* parent, int id, ledmatrix::ledmatrix* display, lib::preset::PresetManager* presets)
    : QWidget(parent)
    , id(id)
  {
    // Initialize QSettings
    settings = new QSettings("fabulouscodingfox", "framework16-led-info-display", this);

    // Layout for the tab
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    // Horizontal slider for FPS
    QHBoxLayout* fpsLayout = new QHBoxLayout();
    QLabel* fpsLabel = new QLabel("FPS: ", this);
    fpsValueLabel = new QLabel("1", this); // Label to display the value
    fpsSlider = new QSlider(Qt::Horizontal, this);
    fpsSlider->setRange(1, 60);
    fpsSlider->setValue(settings->value(QString("%1_fps").arg(id), 24).toInt()); // Load saved value or default to 24
    fpsValueLabel->setText(QString::number(fpsSlider->value()));
    connect(fpsSlider, &QSlider::valueChanged, this, [this](int value)
            { fpsValueLabel->setText(QString::number(value)); });
    fpsLayout->addWidget(fpsLabel);
    fpsLayout->addWidget(fpsSlider);
    fpsLayout->addWidget(fpsValueLabel);
    mainLayout->addLayout(fpsLayout);

    // Save button
    QPushButton* saveButton = new QPushButton("Save", this);
    connect(saveButton, &QPushButton::clicked, this, &SettingsTab::saveSettings);
    mainLayout->addWidget(saveButton);

    setLayout(mainLayout);
  }

  void SettingsTab::saveSettings()
  {
    // Save the FPS value to settings
    settings->setValue(QString("%1_fps").arg(id), fpsSlider->value());
  }

  ConfigWindow::ConfigWindow(std::vector<ledmatrix::ledmatrix*>* displays, lib::preset::PresetManager* presets)
  {
    setWindowTitle(APPLICATION_NAME);
    resize(400, 300);
    setWindowIcon(QIcon(":/resources/icon.png"));

    // Create a QTabWidget
    QTabWidget* tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    for (int i = 0; i < displays->size(); i++)
    {
      tabWidget->addTab(new SettingsTab(this, i, displays->at(i), presets), QString("Display %1").arg(i + 1));
    }

    // Ensure tabs fill the window
    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Optional: Set the tab position (top, bottom, left, or right)
    tabWidget->setTabPosition(QTabWidget::North);
  }

  TrayApp::TrayApp(std::vector<ledmatrix::ledmatrix*>* displays, lib::preset::PresetManager* presets)
    : displays(displays)
    , presets(presets)
  {
    trayIcon = new QSystemTrayIcon(QIcon(":/resources/icon.png"), this);
    trayIcon->setToolTip(APPLICATION_NAME);

    // Create the menu and actions
    auto* menu = new QMenu();
    auto* openAction = new QAction("Open", this);
    auto* quitAction = new QAction("Quit", this);

    menu->addAction(openAction);
    menu->addAction(quitAction);
    trayIcon->setContextMenu(menu);

    // Connect actions to their slots
    connect(openAction, &QAction::triggered, this, &TrayApp::showWindow);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    // Connect tray icon signals
    connect(trayIcon, &QSystemTrayIcon::activated, this, &TrayApp::onTrayIconActivated);

    // Show the tray icon
    trayIcon->show();
  }

  TrayApp::~TrayApp()
  {
    delete trayIcon;
  }

  void TrayApp::showWindow()
  {
    if (!window)
    {
      window = new ConfigWindow(displays, presets);
      connect(window, &QWidget::destroyed, this, [this]()
              { window = nullptr; });
    }
    window->show();
    window->raise(); // Bring the window to the front
    window->activateWindow();
  }

  void TrayApp::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
  {
    if (reason == QSystemTrayIcon::Trigger)
    { // Left-click on tray icon
      showWindow();
    }
  }

  // NOLINTNEXTLINE
  auto run(int argc, char* argv[], std::vector<ledmatrix::ledmatrix*>* displays, lib::preset::PresetManager* presets) -> int
  {
    QApplication app(argc, argv);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, []()
                     { lib::usb::exit(); });

    TrayApp trayApp(displays, presets);

    return app.exec();
  };
} // namespace lib::window
