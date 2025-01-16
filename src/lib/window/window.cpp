#include "./window.hpp"
#include "../usb/usb.hpp"
#include <QAction>
#include <QApplication>
#include <QComboBox>
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
  SettingsTab::SettingsTab(QWidget* parent, ledmatrix::ledmatrix* display, lib::preset::PresetManager* presets, QSettings* settings)
    : QWidget(parent)
    , id(display->id)
    , display(display)
    , settings(settings)
    , presets(presets)
  {
    // Layout for the tab
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    // Horizontal slider for FPS
    QHBoxLayout* fpsLayout = new QHBoxLayout();
    QLabel* fpsLabel = new QLabel("FPS: ", this);
    fpsValueLabel = new QLabel("", this); // Label to display the value
    fpsSlider = new QSlider(Qt::Horizontal, this);
    fpsSlider->setRange(1, 60);
    fpsSlider->setValue(settings->value(QString("%1_fps").arg(id), 24).toInt());
    fpsValueLabel->setText(QString::number(fpsSlider->value()));
    connect(fpsSlider, &QSlider::valueChanged, this, [this](int value)
            { fpsValueLabel->setText(QString::number(value)); });
    fpsLayout->addWidget(fpsLabel);
    fpsLayout->addWidget(fpsSlider);
    fpsLayout->addWidget(fpsValueLabel);
    mainLayout->addLayout(fpsLayout);

    // Horizontal slider for brightness
    QHBoxLayout* brightnessLayout = new QHBoxLayout();
    QLabel* brightnessLabel = new QLabel("Brightness: ", this);
    brightnessValueLabel = new QLabel("", this); // Label to display the value
    brightnessSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setRange(0, 255);
    brightnessSlider->setValue(settings->value(QString("%1_brightness").arg(id), 150).toInt());
    brightnessValueLabel->setText(QString::number(brightnessSlider->value()));
    connect(brightnessSlider, &QSlider::valueChanged, this, [this](int value)
            { brightnessValueLabel->setText(QString::number(value)); });
    brightnessLayout->addWidget(brightnessLabel);
    brightnessLayout->addWidget(brightnessSlider);
    brightnessLayout->addWidget(brightnessValueLabel);
    mainLayout->addLayout(brightnessLayout);

    // Dropdown for presets
    QLabel* presetLabel = new QLabel("Preset: ", this);
    presetComboBox = new QComboBox(this);
    presetComboBox->addItem("None", "none");
    for (const auto& preset : presets->list())
    {
      QString presetName = QString::fromStdString(preset->get_name());
      QString presetId = QString::fromStdString(preset->get_id());
      presetComboBox->addItem(presetName, presetId);
    }
    QString savedKey = settings->value(QString("%1_preset").arg(id), "none").toString();
    int index = presetComboBox->findData(savedKey);
    if (index != -1)
    {
      presetComboBox->setCurrentIndex(index);
    }
    mainLayout->addWidget(presetLabel);
    mainLayout->addWidget(presetComboBox);

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
    settings->setValue(QString("%1_brightness").arg(id), brightnessSlider->value());
    settings->setValue(QString("%1_preset").arg(id), presetComboBox->currentData().toString());
    display->apply_config(settings, presets);
  }

  ConfigWindow::ConfigWindow(std::vector<ledmatrix::ledmatrix*>* displays, lib::preset::PresetManager* presets, QSettings* settings)
  {
    setWindowTitle(APPLICATION_NAME);
    resize(400, 300);
    setWindowIcon(QIcon(":/resources/icon.png"));

    // Create a QTabWidget
    QTabWidget* tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    for (int i = 0; i < displays->size(); i++)
    {
      tabWidget->addTab(new SettingsTab(this, displays->at(i), presets, settings), QString("Display %1").arg(displays->at(i)->id));
    }

    // Ensure tabs fill the window
    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Optional: Set the tab position (top, bottom, left, or right)
    tabWidget->setTabPosition(QTabWidget::North);
  }

  TrayApp::TrayApp(std::vector<ledmatrix::ledmatrix*>* displays, lib::preset::PresetManager* presets, QSettings* settings)
    : displays(displays)
    , presets(presets)
    , settings(settings)
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
      window = new ConfigWindow(displays, presets, settings);
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

    QSettings settings("fabulouscodingfox", "framework16-led-info-display");

    for (auto display : *displays)
    {
      display->apply_config(&settings, presets);
    }

    TrayApp trayApp(displays, presets, &settings);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [displays]()
                     {
      for (auto display : *displays)
      {
        if (display->preset != nullptr)
        {
          display->preset->render(display);
        }
      } });
    timer.start(1000 / 60);

    return app.exec();
  };
} // namespace lib::window
