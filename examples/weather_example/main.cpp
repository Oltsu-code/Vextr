#include <Vextr/App.hpp>
#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/widgets/Button.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Input.hpp>
#include <Vextr/widgets/Label.hpp>
#include <Vextr/widgets/TextView.hpp>

#include <algorithm>
#include <array>
#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

using namespace vextr::core;
using namespace vextr::layout;
using namespace vextr::widgets;

namespace {

struct WeatherData {
  bool ok = false;
  std::string city;
  std::string tempC;
  std::string windKmh;
  std::string weather;
  std::string observationTime;
  std::vector<std::string> forecastLines;
  std::string error;
};

std::string trim(const std::string &value) {
  const auto begin = value.find_first_not_of(" \t\r\n");
  if (begin == std::string::npos) {
    return "";
  }
  const auto end = value.find_last_not_of(" \t\r\n");
  return value.substr(begin, end - begin + 1);
}

std::string sanitizeCity(const std::string &value) {
  std::string out;
  out.reserve(value.size());
  for (const char c : value) {
    const bool isAlphaNum =
        (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9');
    if (isAlphaNum || c == ' ' || c == ',' || c == '-' || c == '.') {
      out.push_back(c);
    }
  }
  return trim(out);
}

std::string shellQuote(const std::string &value) {
  std::string escaped;
  escaped.reserve(value.size() + 8);
  for (const char c : value) {
    if (c == '"' || c == '\\') {
      escaped.push_back('\\');
    }
    escaped.push_back(c);
  }
  return '"' + escaped + '"';
}

std::string runCommand(const std::string &command, int &exitCode) {
  exitCode = -1;
  std::array<char, 512> buffer{};
  std::string output;

  FILE *pipe = POPEN(command.c_str(), "r");
  if (!pipe) {
    output = "ERROR=Failed to launch Ruby process. Is Ruby installed and in PATH?\n";
    return output;
  }

  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) !=
         nullptr) {
    output += buffer.data();
  }

  exitCode = PCLOSE(pipe);
  return output;
}

WeatherData parseWeatherOutput(const std::string &output) {
  WeatherData result;

  std::istringstream in(output);
  std::string line;
  bool inForecast = false;

  while (std::getline(in, line)) {
    line = trim(line);
    if (line.empty()) {
      continue;
    }

    if (line == "OK") {
      result.ok = true;
      continue;
    }

    if (line.rfind("ERROR=", 0) == 0) {
      result.ok = false;
      result.error = trim(line.substr(6));
      continue;
    }

    if (line == "FORECAST:") {
      inForecast = true;
      continue;
    }

    if (line == "END_FORECAST") {
      inForecast = false;
      continue;
    }

    if (inForecast) {
      std::replace(line.begin(), line.end(), '|', ' ');
      result.forecastLines.push_back(line);
      continue;
    }

    auto separator = line.find('=');
    if (separator == std::string::npos) {
      continue;
    }

    const std::string key = line.substr(0, separator);
    const std::string value = line.substr(separator + 1);

    if (key == "CITY") {
      result.city = value;
    } else if (key == "TEMP_C") {
      result.tempC = value;
    } else if (key == "WIND_KMH") {
      result.windKmh = value;
    } else if (key == "WEATHER") {
      result.weather = value;
    } else if (key == "OBS_TIME") {
      result.observationTime = value;
    }
  }

  if (result.ok && result.city.empty()) {
    result.ok = false;
    result.error = "Weather service returned an incomplete response.";
  }

  return result;
}

std::string buildWeatherCommand(const std::string &city) {
  const std::string cleanCity = sanitizeCity(city);
  if (cleanCity.empty()) {
    return "";
  }

#ifndef WEATHER_RUBY_SCRIPT_PATH
#error "WEATHER_RUBY_SCRIPT_PATH compile definition is required"
#endif

  std::string command = "ruby ";
  command += shellQuote(WEATHER_RUBY_SCRIPT_PATH);
  command += " --city ";
  command += shellQuote(cleanCity);
  return command;
}

} // namespace

int main() {
  vextr::App app;

  auto root = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Vertical, 1));
  root->setStyle(Style{.bg = {8, 12, 26}, .padding = PaddingSpec{1, 1, 1, 1}});

  auto title = std::make_shared<Label>(
      "Weather App (Ruby + Vextr) - Enter city and fetch latest weather");
  title->setStyle(Style{.fg = {198, 228, 255}, .bg = {8, 12, 26}, .text = {.bold = true}});

  auto inputHint = std::make_shared<Label>("City");
  inputHint->setStyle(Style{.fg = {160, 188, 214}, .bg = {8, 12, 26}});

  auto cityInput = std::make_shared<Input>("Helsinki");
  cityInput->setText("Helsinki");
  cityInput->setStyle(Style{
      .fg = {240, 246, 255},
      .bg = {20, 30, 56},
      .border = {.style = BorderStyle::Single, .color = {70, 96, 140}},
      .padding = PaddingSpec{0, 1, 0, 1},
  });
  cityInput->setFocusedStyle(Style{
      .fg = {255, 255, 255},
      .bg = {32, 48, 86},
      .border = {.style = BorderStyle::Double, .color = {120, 164, 220}},
      .padding = PaddingSpec{0, 1, 0, 1},
  });

  auto status = std::make_shared<Label>("Ready. Press Enter in city field or use Fetch.");
  status->setStyle(Style{.fg = {150, 176, 202}, .bg = {8, 12, 26}});

  auto weatherCard = std::make_shared<TextView>(
      "Current weather will appear here after fetching data.");
  weatherCard->setFlowMode(TextFlowMode::Wrap);
  weatherCard->setStyle(Style{
      .fg = {228, 239, 255},
      .bg = {16, 24, 44},
      .border = {.style = BorderStyle::Single, .color = {78, 112, 168}},
      .padding = PaddingSpec{1, 1, 1, 1},
  });

  auto forecastCard = std::make_shared<TextView>(
      "5-day forecast will appear here after fetching data.");
  forecastCard->setFlowMode(TextFlowMode::Wrap);
  forecastCard->setStyle(Style{
      .fg = {228, 239, 255},
      .bg = {16, 24, 44},
      .border = {.style = BorderStyle::Single, .color = {78, 112, 168}},
      .padding = PaddingSpec{1, 1, 1, 1},
  });

  auto controls = std::make_shared<Container>(
      std::make_shared<StackLayout>(Axis::Horizontal, 1));
  controls->setStyle(Style{.bg = {8, 12, 26}});

  auto fetchBtn = std::make_shared<Button>("Fetch Weather");
  fetchBtn->setStyle(Style{.fg = {245, 250, 255}, .bg = {34, 92, 68}});
  fetchBtn->setFocusedStyle(Style{
      .fg = {255, 255, 255}, .bg = {52, 128, 92}, .text = {.bold = true}});

  auto quitBtn = std::make_shared<Button>("Quit");
  quitBtn->setStyle(Style{.fg = {255, 230, 230}, .bg = {100, 44, 44}});
  quitBtn->setFocusedStyle(Style{
      .fg = {255, 255, 255}, .bg = {146, 64, 64}, .text = {.bold = true}});

  auto fetchWeather = [&]() {
    const std::string city = cityInput->getText();
    const std::string cmd = buildWeatherCommand(city);

    if (cmd.empty()) {
      status->setText("Please enter a valid city name.");
      return;
    }

    status->setText("Fetching weather for: " + sanitizeCity(city) + " ...");

    int exitCode = -1;
    const std::string output = runCommand(cmd, exitCode);
    const WeatherData data = parseWeatherOutput(output);

    if (exitCode != 0 || !data.ok) {
      status->setText("Weather fetch failed.");
      std::string details = "Could not fetch weather data.\n";
      if (!data.error.empty()) {
        details += "Error: " + data.error + "\n";
      }
      details += "Ensure Ruby is installed and internet is available.";
      weatherCard->setText(details);
      forecastCard->setText("No forecast available.");
      return;
    }

    status->setText("Updated weather for " + data.city + ".");

    std::string current;
    current += "City: " + data.city + "\n";
    current += "Condition: " + data.weather + "\n";
    current += "Temperature: " + data.tempC + " C\n";
    current += "Wind: " + data.windKmh + " km/h\n";
    current += "Observed: " + data.observationTime;
    weatherCard->setText(current);

    std::string forecast = "Date Max Min Condition\n";
    for (const auto &line : data.forecastLines) {
      forecast += line + "\n";
    }
    if (data.forecastLines.empty()) {
      forecast += "No forecast returned by service.";
    }
    forecastCard->setText(forecast);
  };

  fetchBtn->setOnClick(fetchWeather);
  cityInput->setOnSubmit([&](const std::string &) { fetchWeather(); });
  quitBtn->setOnClick([&]() { app.quit(); });

  controls->addChild(fetchBtn, {.ratio = 1.0f});
  controls->addChild(quitBtn, {.ratio = 1.0f});

  root->addChild(title, {.fixedH = cells(1)});
  root->addChild(inputHint, {.fixedH = cells(1)});
  root->addChild(cityInput, {.fixedH = cells(1)});
  root->addChild(status, {.fixedH = cells(1)});
  root->addChild(weatherCard, {.ratio = 1.0f});
  root->addChild(forecastCard, {.ratio = 1.0f});
  root->addChild(controls, {.fixedH = cells(1)});

  app.setRoot(root);
  app.run();
  return 0;
}