
#include <string>
#include <optional>

#include "catch_test_macros.hpp"

#include "cppli.h"
#include "test_utils.h"

using namespace cppli;
using namespace cppli::detail;

static bool foo_flag;
static bool bar_flag;
static bool baz_flag;

static std::optional<std::string> color_option;
static std::optional<int> size_option;

CPPLI_SUBCOMMAND(opttest,
                 "takes some flags and some options",

                 CPPLI_FLAG(foo, "foo flag", f),
                 CPPLI_FLAG(bar, "bar flag", b),
                 CPPLI_FLAG(baz, "baz flag", z),

                 CPPLI_OPTION(std::string, color, "color", "color option", c),
                 CPPLI_OPTION(int, size, "color", "color option", s)) {

    foo_flag = foo;
    bar_flag = bar;
    baz_flag = baz;

    //if(color) {
        color_option = color;
    //}

    //if(size) {
        size_option = size;
    //}
}

CPPLI_SUBCOMMAND(reqopttest,
                 "takes one required option",

                 CPPLI_REQUIRED_OPTION(int, size, "size", "size option", s)) {

    size_option = size;
}

static bool was_included;
CPPLI_SUBCOMMAND(optargopttest,
                 "takes one option with an optional argument",

                 CPPLI_OPTIONAL_ARGUMENT_OPTION(std::string, color, "color", "color option", c)) {


    was_included = color.was_included();

                // implicit cast to std::optional
    color_option = color;
}



TEST_CASE("passing a single option works") {
    SECTION("with long name") {
        SECTION("with equals syntax") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            color_option = std::nullopt;
            size_option  = std::nullopt;

            const char* argv[] = {"program", "opttest", "--size=32"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(!foo_flag);
            REQUIRE(!bar_flag);
            REQUIRE(!baz_flag);

            REQUIRE(!color_option);

            REQUIRE(size_option);
            REQUIRE(*size_option == 32);
        }

        SECTION("with space syntax") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            color_option = std::nullopt;
            size_option  = std::nullopt;

            const char* argv[] = {"program", "opttest", "--size", "32"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(!foo_flag);
            REQUIRE(!bar_flag);
            REQUIRE(!baz_flag);

            REQUIRE(!color_option);

            REQUIRE(size_option);
            REQUIRE(*size_option == 32);
        }
    }

    SECTION("with short name") {
        SECTION("with equals syntax") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            color_option = std::nullopt;
            size_option  = std::nullopt;

            const char* argv[] = {"program", "opttest", "-s=32"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(!foo_flag);
            REQUIRE(!bar_flag);
            REQUIRE(!baz_flag);

            REQUIRE(!color_option);

            REQUIRE(size_option);
            REQUIRE(*size_option == 32);
        }

        SECTION("with space syntax") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            color_option = std::nullopt;
            size_option  = std::nullopt;

            const char* argv[] = {"program", "opttest", "-s", "32"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(!foo_flag);
            REQUIRE(!bar_flag);
            REQUIRE(!baz_flag);

            REQUIRE(!color_option);

            REQUIRE(size_option);
            REQUIRE(*size_option == 32);
        }

        SECTION("with connected syntax") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            color_option = std::nullopt;
            size_option  = std::nullopt;

            const char* argv[] = {"program", "opttest", "-s32"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(!foo_flag);
            REQUIRE(!bar_flag);
            REQUIRE(!baz_flag);

            REQUIRE(!color_option);

            REQUIRE(size_option);
            REQUIRE(*size_option == 32);
        }
    }
}

TEST_CASE("passing an option at the end of a flag group works") {
    SECTION("with equals syntax") {
        foo_flag = false;
        bar_flag = false;
        baz_flag = false;

        color_option = std::nullopt;
        size_option  = std::nullopt;

        const char* argv[] = {"program", "opttest", "-fbzs=32"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(foo_flag);
        REQUIRE(bar_flag);
        REQUIRE(baz_flag);

        REQUIRE(!color_option);

        REQUIRE(size_option);
        REQUIRE(*size_option == 32);
    }

    SECTION("with space syntax") {
        foo_flag = false;
        bar_flag = false;
        baz_flag = false;

        color_option = std::nullopt;
        size_option  = std::nullopt;

        const char* argv[] = {"program", "opttest", "-fbzs", "32"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(foo_flag);
        REQUIRE(bar_flag);
        REQUIRE(baz_flag);

        REQUIRE(!color_option);

        REQUIRE(size_option);
        REQUIRE(*size_option == 32);
    }

    SECTION("with connected syntax") {
        foo_flag = false;
        bar_flag = false;
        baz_flag = false;

        color_option = std::nullopt;
        size_option  = std::nullopt;

        const char* argv[] = {"program", "opttest", "-fbzs32"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(foo_flag);
        REQUIRE(bar_flag);
        REQUIRE(baz_flag);

        REQUIRE(!color_option);

        REQUIRE(size_option);
        REQUIRE(*size_option == 32);
    }
}

TEST_CASE("omitting the argument of an option with a required argument throws a user_error") {
    SECTION("with equals syntax") {
        const char* argv[] = {"program", "opttest", "-fbzs="};
        REQUIRE_THREW(user_error, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }

    SECTION("with non-equals syntax") {
        const char* argv[] = {"program", "opttest", "-fbzs"};
        REQUIRE_THREW(user_error, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }
}

TEST_CASE("passing the incorrect type as the argument for an option throw a user_error") {
    const char* argv[] = {"program", "opttest", "--size=foobar"};
    REQUIRE_THREW(user_error, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
}

TEST_CASE("required options work") {
    SECTION("with long name") {
        size_option = std::nullopt;
        const char* argv[] = {"program", "reqopttest", "--size=32"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(size_option);
        REQUIRE(*size_option == 32);
    }

    SECTION("with short name") {
        size_option = std::nullopt;
        const char* argv[] = {"program", "reqopttest", "-s32"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(size_option);
        REQUIRE(*size_option == 32);
    }

    SECTION("not providing a required option throws a user_error") {
        const char* argv[] = {"program", "reqopttest"};
        REQUIRE_THREW(user_error, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }

    SECTION("not providing a required option's argument throws a user_error") {
        SECTION("with long name") {
            const char* argv[] = {"program", "reqopttest", "--size"};
            REQUIRE_THREW(user_error, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
        }

        SECTION("with short name") {
            const char* argv[] = {"program", "reqopttest", "-s"};
            REQUIRE_THREW(user_error, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
        }
    }
}

TEST_CASE("optional argument options work") {

    //was_included = false;
    SECTION("not including option is valid") {
        const char* argv[] = {"program", "optargopttest"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(!was_included);
        REQUIRE(!color_option);
    }

    SECTION("including option without argument is valid and results in was_included being true and an empty optional") {
        SECTION("with long name") {
            const char* argv[] = {"program", "optargopttest", "--color"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(was_included);
            REQUIRE(!color_option);
        }

        SECTION("with short name") {
            const char* argv[] = {"program", "optargopttest", "-c"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(was_included);
            REQUIRE(!color_option);
        }
    }

    SECTION("including option with argument is valid and results in was_included being true and the desired value in the optional") {
        SECTION("with long name") {
            SECTION("with equals syntax") {
                const char* argv[] = {"program", "optargopttest", "--color=blue"};
                cppli::run<"program", "does stuff">(lengthof(argv), argv);

                REQUIRE(was_included);
                REQUIRE(color_option);
                REQUIRE(*color_option == "blue");
            }

            /*SECTION("with space syntax") {
                const char* argv[] = {"program", "optargopttest", "--color", "blue"};
                cppli::run<"program", "does stuff">(lengthof(argv), argv);

                REQUIRE(was_included);
                REQUIRE(color_option);
                REQUIRE(*color_option == "blue");
            }*/
        }

        SECTION("with short name") {
            SECTION("with equals syntax") {
                const char* argv[] = {"program", "optargopttest", "-c=green"};
                cppli::run<"program", "does stuff">(lengthof(argv), argv);

                REQUIRE(was_included);
                REQUIRE(color_option);
                REQUIRE(*color_option == "green");

                SECTION("works when the argument contains '='") {
                    const char* argv[] = {"program", "optargopttest", "-c=="};
                    cppli::run<"program", "does stuff">(lengthof(argv), argv);

                    REQUIRE(was_included);
                    REQUIRE(color_option);
                    REQUIRE(*color_option == "=");
                }
            }

            /*SECTION("with space syntax") {
                const char* argv[] = {"program", "optargopttest", "-c", "blue"};
                cppli::run<"program", "does stuff">(lengthof(argv), argv);

                REQUIRE(was_included);
                REQUIRE(color_option);
                REQUIRE(*color_option == "blue");
            }*/

            SECTION("with connected syntax") {
                const char* argv[] = {"program", "optargopttest", "-cred"};
                cppli::run<"program", "does stuff">(lengthof(argv), argv);

                REQUIRE(was_included);
                REQUIRE(color_option);
                REQUIRE(*color_option == "red");

                SECTION("works when the argument contains '='") {
                    const char* argv[] = {"program", "optargopttest", "-co="};
                    cppli::run<"program", "does stuff">(lengthof(argv), argv);

                    REQUIRE(was_included);
                    REQUIRE(color_option);
                    REQUIRE(*color_option == "=");
                }
            }
        }
    }
}