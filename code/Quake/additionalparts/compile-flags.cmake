add_compile_flags(LD
        -nostartfiles
        -static
        -Wl,--gc-sections
        -Wl,-static
        -Wl,--start-group
        -Wl,--whole-archive
        -Wl,--no-whole-archive
        -Wl,--end-group
        -Wl,-EL
        -Wl,--no-relax
        -T ${SDK_ROOT}/lds/kendryte.ld
        )

# C Flags Settings
add_compile_flags(BOTH
        -mcmodel=medany
        -fno-common
        -ffunction-sections
        -fdata-sections
	-fno-exceptions
        -fstrict-volatile-bitfields
        -fno-zero-initialized-in-bss
        -Os
        -ggdb
        )

add_compile_flags(C -std=gnu11 -Wno-pointer-to-int-cast)
add_compile_flags(CXX -std=gnu++17)

if (BUILDING_SDK)
    add_compile_flags(BOTH
            -Wall
	    #            -Werror=all
	    -Wno-stringop-truncation
	    -Wno-error=maybe-uninitialized
	    -Wno-error=nonnull
	    -Wno-error=parentheses
	    -Wno-error=unused-value
	    -Wno-error=format-overflow
            -Wno-error=unused-function
            -Wno-error=unused-but-set-variable
            -Wno-error=unused-variable
            -Wno-error=deprecated-declarations
	    -Wno-error=restrict
	    -Wno-error=trigraphs
            -Wextra
            -Wno-unused-parameter
            -Wno-sign-compare
            -Wno-error=missing-braces
            -Wno-error=return-type
            -Wno-error=pointer-sign
            -Wno-missing-braces
            -Wno-strict-aliasing
            -Wno-implicit-fallthrough
            -Wno-missing-field-initializers
            -Wno-int-to-pointer-cast
            )

    add_compile_flags(C -Wno-old-style-declaration)
else ()
    add_compile_flags(BOTH -L${SDK_ROOT}/include/)
endif ()
