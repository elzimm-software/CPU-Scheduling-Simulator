.PHONY: all install uninstall clean

CXX := clang++
CXXFLAGS := -g -Wall
LDLIBS :=

BUILD_DIR := build
SOURCE_DIR := src
CPPFLAGS := -I$(SOURCE_DIR)
INSTALL_DIR := /usr/local/bin
INSTALL_MASK := 0755

TARGET := disched

SRCS := $(wildcard $(SOURCE_DIR)/*.cpp)

OBJS := $(SRCS:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(SRCS:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%.d)
DFLAGS := -MMD -MP

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

install: $(BUILD_DIR)/$(TARGET)
	install -m $(INSTALL_MASK) $< $(DESTDIR)$(INSTALL_DIR)/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(INSTALL_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
