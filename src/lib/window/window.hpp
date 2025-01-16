#pragma once

#include "../ledmatrix/ledmatrix.hpp"
#include "../preset.hpp"
#include <QLabel>
#include <QMainWindow>
#include <QSettings>
#include <QSlider>
#include <QString>
#include <QSystemTrayIcon>
#include <QWidget>
#include <vector>

namespace lib::window
{
  static const QString APPLICATION_NAME = "Framework 16 LED Matrix Display";

  class SettingsTab : public QWidget
  {
    Q_OBJECT

  public:
    explicit SettingsTab(QWidget* parent, int id, ledmatrix::ledmatrix* display, lib::preset::PresetManager* presets);

  private slots:
    void saveSettings();

  private:
    QSlider* fpsSlider;
    QLabel* fpsValueLabel;
    QSettings* settings;
    int id;
  };

  class ConfigWindow : public QMainWindow
  {
    Q_OBJECT

  public:
    ConfigWindow(std::vector<ledmatrix::ledmatrix*>* displays, lib::preset::PresetManager* presets);
    ~ConfigWindow() override = default;
  };

  class TrayApp : public QObject
  {
    Q_OBJECT

  public:
    TrayApp(std::vector<ledmatrix::ledmatrix*>* displays, lib::preset::PresetManager* presets);
    ~TrayApp() override;

  private slots:
    void showWindow();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

  private:
    QSystemTrayIcon* trayIcon = nullptr;
    ConfigWindow* window = nullptr;
    std::vector<ledmatrix::ledmatrix*>* displays;
    lib::preset::PresetManager* presets;
  };

  // NOLINTNEXTLINE
  auto run(int argc, char* argv[], std::vector<ledmatrix::ledmatrix*>* displays, lib::preset::PresetManager* presets) -> int;
} // namespace lib::window