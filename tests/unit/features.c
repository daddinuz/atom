/*
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 *
 * Copyright (c) 2018 Davide Di Carlo
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <atom.h>
#include <stdint.h>
#include <traits/traits.h>
#include "features.h"

struct ByteArray {
    const char *bytes;
    const size_t size;
};

Feature(fromBytes) {
    Atom sut = NULL;

    const struct ByteArray bytesArray[] = {
            {.bytes="", .size=0},
            {.bytes="Hello world!", .size=12},
            {.bytes="sit,\0amet,\0consectetur,\0adipisci", .size=32}
    };
    const size_t bytesArraySize = sizeof(bytesArray) / sizeof(bytesArray[0]);

    for (size_t i = 0; i < bytesArraySize; i++) {
        const void *bytes = bytesArray[i].bytes;
        const size_t size = bytesArray[i].size;

        sut = Atom_fromBytes(bytes, size);

        assert_equal(size, Atom_length(sut));
        assert_memory_equal(size, bytes, sut);

        assert_equal(sut, Atom_fromBytes(bytes, size));  // ensure instance reusing
    }
}

Feature(fromBytes_checkRuntimeErrors) {
    Atom sut = NULL;
    const void *bytes = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Atom_fromBytes(bytes, 8);
        (void) sut;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    traits_unit_wraps(SIGABRT) {
        sut = Atom_fromBytes(&bytes, SIZE_MAX);
        (void) sut;
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
}

Feature(fromLiteral) {
    Atom sut = NULL;
    const char *literals[] = {"", "Hello world!", "lorem ipsum dolor", "sit,\0amet,\0consectetur,\0adipisci"};
    const size_t literalsSize = sizeof(literals) / sizeof(literals[0]);

    for (size_t i = 0; i < literalsSize; i++) {
        const char *literal = literals[i];
        const size_t length = strlen(literal);

        sut = Atom_fromLiteral(literal);

        assert_equal(length, Atom_length(sut));
        assert_string_equal(literal, sut);

        assert_equal(sut, Atom_fromLiteral(literal));  // ensure instance reusing
    }
}

Feature(fromLiteral_checkRuntimeErrors) {
    Atom sut = NULL;
    const char *literal = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Atom_fromLiteral(literal);
        (void) sut;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(fromInteger) {
    Atom sut = NULL;
    struct { long n; char *s; } seeds[] = {
            {.n=0, .s="0"},
            {.n=1000, .s="1000"},
            {.n=-1000, .s="-1000"},
            {.n=2765463013, .s="2765463013"},
            {.n=-2765463013, .s="-2765463013"},
    };
    const size_t seedsSize = sizeof(seeds) / sizeof(seeds[0]);

    for (size_t i = 0; i < seedsSize; i++) {
        const long number = seeds[i].n;
        const char *string = seeds[i].s;
        const size_t length = strlen(string);

        sut = Atom_fromInteger(number);

        assert_equal(length, Atom_length(sut));
        assert_string_equal(string, sut);

        assert_equal(sut, Atom_fromInteger(number));  // ensure instance reusing
    }
}

Feature(fromFloating) {
    Atom sut = NULL;
    struct { long double n; char *s; } seeds[] = {
            {.n=0.0, .s="0.00000000"},
            {.n=10.87, .s="10.87000000"},
            {.n=-10.87, .s="-10.87000000"},
            {.n=27654.6313, .s="27654.63130000"},
            {.n=-27654.6313, .s="-27654.63130000"},
    };
    const size_t seedsSize = sizeof(seeds) / sizeof(seeds[0]);

    for (size_t i = 0; i < seedsSize; i++) {
        const long double number = seeds[i].n;
        const char *string = seeds[i].s;
        const size_t length = strlen(string);

        sut = Atom_fromFloating(number);

        printf("%s: %s\n", string, sut);
        assert_equal(length, Atom_length(sut));
        assert_string_equal(string, sut);

        assert_equal(sut, Atom_fromFloating(number));  // ensure instance reusing
    }
}

Feature(length_checkRuntimeErrors) {
    Atom sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        const size_t length = Atom_length(sut);
        (void) length;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    traits_unit_wraps(SIGABRT) {
        const size_t length = Atom_length("not a valid atom");
        (void) length;
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
}

Feature(equals) {
    assert_true(Atom_equals(Atom_fromBytes("lo\0rem", sizeof("lo\0rem")), Atom_fromBytes("lo\0rem", sizeof("lo\0rem"))));
    assert_false(Atom_equals(Atom_fromBytes("lo\0rom", sizeof("lo\0rom")), Atom_fromBytes("lo\0rem", sizeof("lo\0rem"))));

    assert_true(Atom_equals(Atom_fromLiteral("lorem"), Atom_fromLiteral("lorem")));
    assert_false(Atom_equals(Atom_fromLiteral("lorom"), Atom_fromLiteral("lorem")));

    assert_true(Atom_equals(Atom_fromInteger(8756365), Atom_fromInteger(8756365)));
    assert_false(Atom_equals(Atom_fromInteger(898677), Atom_fromInteger(8756865)));

    assert_true(Atom_equals(Atom_fromFloating(875.6365), Atom_fromFloating(875.6365)));
    assert_false(Atom_equals(Atom_fromFloating(834.2495), Atom_fromFloating(875.6365)));
}

Feature(equals_checkRuntimeErrors) {
    Atom sut = Atom_fromLiteral("lorem"), other = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        assert_true(Atom_equals(other, sut));
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    traits_unit_wraps(SIGABRT) {
        assert_true(Atom_equals(sut, other));
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());

    traits_unit_wraps(SIGABRT) {
        assert_true(Atom_equals(sut, "not a valid atom instance"));
    }

    assert_equal(counter + 3, traits_unit_get_wrapped_signals_counter());

    traits_unit_wraps(SIGABRT) {
        assert_true(Atom_equals("not a valid atom instance", sut));
    }

    assert_equal(counter + 4, traits_unit_get_wrapped_signals_counter());
}
