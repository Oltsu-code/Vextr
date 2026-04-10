#include <gtest/gtest.h>
#include <Vextr/App.hpp>
#include <Vextr/widgets/Label.hpp>
#include <memory>

namespace vextr {

class AppTest : public ::testing::Test {
protected:
    App app;
};

TEST_F(AppTest, AppDefaultConstruction) {
    // App should construct without errors
    EXPECT_TRUE(&app);
}

TEST_F(AppTest, SetRootWidget) {
    auto label = std::make_shared<widgets::Label>("Test");
    EXPECT_NO_THROW(app.setRoot(label));
}

TEST_F(AppTest, SetNullRoot) {
    EXPECT_NO_THROW(app.setRoot(nullptr));
}

TEST_F(AppTest, QuitMethod) {
    // Verify quit method exists and doesn't throw
    EXPECT_NO_THROW(app.quit());
}

TEST_F(AppTest, SetMultipleRoots) {
    auto label1 = std::make_shared<widgets::Label>("Label 1");
    auto label2 = std::make_shared<widgets::Label>("Label 2");

    EXPECT_NO_THROW(app.setRoot(label1));
    EXPECT_NO_THROW(app.setRoot(label2));
}

TEST_F(AppTest, EscapeTimerInitialization) {
    EXPECT_EQ(app.escapeTimer, 0);
}

TEST_F(AppTest, InputReaderAccess) {
    // Verify inputReader is accessible
    EXPECT_TRUE(&app.inputReader);
}

TEST_F(AppTest, InputParserAccess) {
    // Verify inputParser is accessible
    EXPECT_TRUE(&app.inputParser);
}

TEST_F(AppTest, RendererAccess) {
    // Verify renderer is accessible
    EXPECT_TRUE(&app.renderer);
}

} // namespace vextr
