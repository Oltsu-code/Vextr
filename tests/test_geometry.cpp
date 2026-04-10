#include <gtest/gtest.h>
#include <Vextr/core/Geometry.hpp>

namespace vextr::core {

class GeometryTest : public ::testing::Test {
protected:
    Size size;
    Pos pos;
    Rect rect;
};

TEST_F(GeometryTest, SizeDefaultConstruction) {
    EXPECT_EQ(size.width, 0);
    EXPECT_EQ(size.height, 0);
}

TEST_F(GeometryTest, SizeWithValues) {
    Size s{80, 24};
    EXPECT_EQ(s.width, 80);
    EXPECT_EQ(s.height, 24);
}

TEST_F(GeometryTest, PosDefaultConstruction) {
    EXPECT_EQ(pos.x, 0);
    EXPECT_EQ(pos.y, 0);
}

TEST_F(GeometryTest, PosWithValues) {
    Pos p{10, 5};
    EXPECT_EQ(p.x, 10);
    EXPECT_EQ(p.y, 5);
}

TEST_F(GeometryTest, RectDefaultConstruction) {
    EXPECT_EQ(rect.x, 0);
    EXPECT_EQ(rect.y, 0);
    EXPECT_EQ(rect.width, 0);
    EXPECT_EQ(rect.height, 0);
}

TEST_F(GeometryTest, RectWithValues) {
    Rect r{5, 10, 40, 20};
    EXPECT_EQ(r.x, 5);
    EXPECT_EQ(r.y, 10);
    EXPECT_EQ(r.width, 40);
    EXPECT_EQ(r.height, 20);
}

TEST_F(GeometryTest, RectRightBoundary) {
    Rect r{5, 10, 40, 20};
    EXPECT_EQ(r.right(), 45);  // 5 + 40
}

TEST_F(GeometryTest, RectBottomBoundary) {
    Rect r{5, 10, 40, 20};
    EXPECT_EQ(r.bottom(), 30);  // 10 + 20
}

TEST_F(GeometryTest, RectBoundariesZeroSize) {
    Rect r{0, 0, 0, 0};
    EXPECT_EQ(r.right(), 0);
    EXPECT_EQ(r.bottom(), 0);
}

TEST_F(GeometryTest, RectBoundariesLargeValues) {
    Rect r{100, 200, 300, 400};
    EXPECT_EQ(r.right(), 400);
    EXPECT_EQ(r.bottom(), 600);
}

} // namespace vextr::core
