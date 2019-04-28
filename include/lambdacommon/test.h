/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

/*
 * LambdaTest
 * A very lightweight testing framework.
 */

#ifndef LAMBDACOMMON_TEST_H
#define LAMBDACOMMON_TEST_H

#include "system/terminal.h"
#include "system/time.h"
#include <memory>
#include <functional>

#define LC_TEST_SECTION(section) \
    namespace lc_test_section_##section { static std::shared_ptr<lambdatest::TestSection> __section__ = lambdatest::__auto_register_section(std::move(std::string(#section))); } \
    namespace lc_test_section_##section

#define LC_TEST(_test_, name) \
    static void __lc_test_##_test_(lambdatest::Test &test); \
    lambdatest::TestAutoRegister __tar_##_test_##__(__section__, std::move(std::string(#name)), __lc_test_##_test_); \
    static void __lc_test_##_test_(lambdatest::Test &test)

#define REQUIRE(...) test.require(__VA_ARGS__, std::string(#__VA_ARGS__), __LINE__, std::string(__FILE__))

namespace lambdatest
{
    using namespace std::rel_ops;
    namespace term = lambdacommon::terminal;

    /*!
     * Represents a very simple test case.
     */
    class Test
    {
    private:
        std::function<void(Test&)> _test_func;
        std::string _name;
        size_t _requirements = 0;
        size_t _requirements_passed = 0;

    public:
        Test(std::string name, std::function<void(Test&)> test_func) : _name(std::move(name)), _test_func(std::move(test_func)) {}

        /*!
         * Gets the name of the test.
         * @return The test's name.
         */
        const std::string& get_name() const {
            return _name;
        }

        /*!
         * Gets the number of requirements to met to pass the test.
         * @return The number of requirements to met.
         */
        size_t get_requirements() const {
            return _requirements;
        }

        /*!
         * Gets the number of requirements passed.
         * @return The number of requirements passed.
         */
        size_t get_requirements_passed() const {
            return _requirements_passed;
        }

        /*!
         * Tests.
         */
        void test() {
            _test_func(*this);
        }

        /*!
         * Adds a requirement to complete the test.
         * @param condition The condition.
         * @param expression The expression of the requirement.
         * @param line The line where the requirement is specified.
         * @param file The file where the requirement is specified.
         */
        void require(bool condition, const std::string& expression, size_t line, const std::string& file) {
            _requirements++;
            if (condition)
                _requirements_passed++;
            else {
                std::cout << term::RED << "  Error: requirement is not met at " << file << ":" << std::to_string(line) << ", expression: " << term::MAGENTA << expression
                          << term::RED << "." << term::RESET << std::endl;
            }
        }

        /*!
         * Checks whether the test pass or not.
         * @return True if the test pass, else false.
         */
        bool pass() const {
            return _requirements_passed == _requirements;
        }

        bool operator==(const Test& other) const {
            return _name == other._name;
        }

        bool operator<(const Test& other) const {
            return _name < other._name;
        }
    };

    /*!
     * Represents a test section.
     */
    class TestSection
    {
    private:
        std::string _name;
        std::vector<Test> _tests;

        void print_section_result(size_t passed) {
            std::cout << "=> ";
            if (_tests.size() == passed) std::cout << term::formats({term::BOLD, term::LIGHT_GREEN}) << "PASSED ";
            else std::cout << term::formats({term::BOLD, term::LIGHT_RED}) << "FAILED ";
            std::cout << '(' << std::to_string(passed) << " tests passed out of " << std::to_string(_tests.size()) << ')' << term::RESET << std::endl;
        }

    public:
        TestSection(std::string name) : _name(std::move(name)) {}

        /*!
         * Gets the name of the test section.
         * @return The name of the section.
         */
        const std::string& get_name() const {
            return _name;
        }

        /*!
         * Tests the section.
         * @return The test result.
         */
        bool test() {
            size_t passed_count = 0;
            for (auto& test : _tests) {
                std::cout << "Testing " << term::formats({term::BOLD, term::LIGHT_BLUE}) << test.get_name() << term::RESET << "..." << std::endl;
                test.test();
                std::cout << " -> Result: ";
                if (test.pass()) {
                    std::cout << term::formats({term::LIGHT_GREEN, term::BOLD}) << "PASSED" << term::RESET << std::endl;
                    passed_count++;
                } else if (!test.pass()) {
                    std::cout << term::formats({term::LIGHT_RED, term::BOLD}) << "FAILED (" << std::to_string(test.get_requirements_passed()) << "/"
                              << std::to_string(test.get_requirements()) << ")" << term::RESET << std::endl;
                    print_section_result(passed_count);
                    return false;
                }
            }
            print_section_result(passed_count);
            return true;
        }

        /*!
         * Register a test in the section.
         * @param test The test to register.
         */
        void register_test(Test test) {
            _tests.push_back(std::move(test));
        }

        bool operator==(const TestSection& other) const {
            return _name == other._name;
        }

        bool operator<(const TestSection& other) const {
            return _name < other._name;
        }
    };

    class LambdaTest
    {
    private:
        std::vector<std::shared_ptr<TestSection>> _sections;

        LambdaTest() {}

    public:
        /*!
         * Registers a section in the LambdaTest instance.
         * @param section The section to register.
         */
        void register_section(const std::shared_ptr<TestSection>& section) {
            _sections.push_back(section);
        }

        /*!
         * Launches the tests.
         * @return EXIT_SUCCESS if all tests succeed, else EXIT_FAILURE.
         */
        int launch() {
            std::cout << "Testing " << _sections.size() << " sections..." << std::endl;
            time_t start = lambdacommon::time::get_time_millis();
            size_t sections_passed = 0;
            for (auto& section : _sections) {
                std::cout << term::LIGHT_YELLOW << "========== " << section->get_name() << " section ==========" << term::RESET << std::endl;
                if (section->test()) {
                    sections_passed++;
                }
                std::cout << std::endl;
            }
            time_t end = lambdacommon::time::get_time_millis();

            auto passed_format = term::YELLOW;
            if (sections_passed == 0)
                passed_format = term::RED;
            else if (sections_passed == _sections.size())
                passed_format = term::GREEN;

            std::cout << std::endl;
            std::cout << "TESTS RESULTS: " << std::endl;
            std::cout << passed_format << "  PASSED: " << std::to_string(sections_passed) << '/' << _sections.size() << std::endl;
            std::cout << "  FAILED: " << std::to_string(_sections.size() - sections_passed) << '/' << _sections.size() << term::RESET << std::endl;
            std::cout << term::MAGENTA << "  EXECUTION TIME: " << std::to_string(end - start) << "ms" << term::RESET << std::endl;
            std::cout << std::endl;
            return sections_passed == _sections.size() ? EXIT_SUCCESS : EXIT_FAILURE;
        }

        static LambdaTest INSTANCE;
    };

    LambdaTest LambdaTest::INSTANCE = LambdaTest();

    std::shared_ptr<TestSection> __auto_register_section(std::string name) {
        std::shared_ptr<TestSection> section = std::make_shared<TestSection>(std::move(name));
        LambdaTest::INSTANCE.register_section(section);
        return section;
    }

    struct TestAutoRegister
    {
        TestAutoRegister(std::shared_ptr<TestSection>& section, std::string name, const std::function<void(Test&)>& test_func) {
            section->register_test(std::move(Test(std::move(name), test_func)));
        }
    };
}

#endif //LAMBDACOMMON_TEST_H
