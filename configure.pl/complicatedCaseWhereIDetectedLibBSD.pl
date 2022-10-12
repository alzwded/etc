#!/usr/bin/env perl
# Copyright 2022 Vlad Mesco
# 
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

use strict;
use warnings;
use Getopt::Long;

my %defines=();

my $prefix = "/usr/local";
my $showHelp = 0;
my $etc_conf = "/etc/jakobmenu.conf";
my $home_conf = "~/.config/jakobmenu/conf";
my $libbsdroot = undef;

my $cflags = $ENV{CFLAGS} || "";
my $ldflags = $ENV{LDFLAGS} || "";

GetOptions("prefix=s" => \$prefix,
           "help" => \$showHelp,
           "etc_conf=s" => \$etc_conf,
           "home_conf=s" => \$home_conf,
           "libbsd_root=s" => \$libbsdroot)
           or die("Error parsing command line");

if($showHelp) {
    print <<EOT;
$0 [-prefix=/usr/local]
    -prefix=/usr/local                     set prefix for install
    -help                                  print this message
    -etc_conf=/etc/jakobmenu.conf          default /etc conf path
    -user_conf=\\~/.config/jakobmenu/conf   default user config path
                                           a quoted ~ will be expanded
                                           to the user's home directory
                                           at runtime.
    -libbsd_root=/home/user/libbsd         if your system doesn't have
                                           native support for sys/tree.h,
                                           consider libbsd; if you can't
                                           install it system-wide, use
                                           this flag to point it to where
                                           it is implemented

This script generates Makefile.vars and config.h to generate sensible
defaults based on what is detected in the environment, or your whims.

This script inspects the following environment variables and saves them
to Makefile.vars if defined:
    CC              C compiler
    CFLAGS          additional flags to the \${CC} invocation
    LDFLAGS         linker flags passed to \${CC}

If CC is not defined, it will try, in order, 'cc', 'gcc' and 'clang'.
EOT
    exit(1);
}

sub compile {
    my ($what, $ccode, $cc, $lambda, $ildflags) = @_;
    my $ldflags = $ildflags || "";

    print "Checking $what...\n";
    open A, ">test.c";
    print A $ccode;

    my $compiles = system("$cc test.c $ldflags") ? 0 : 1;
    my $exitCode = -1;
    if($compiles) {
        $exitCode = system("./a.out") >> 8;
    }

    close A;
    unlink("test.c");
    unlink("a.out");

    return &$lambda($compiles, $exitCode);
}

####################################################################
# checks
####################################################################

sub tryCompiler {
    my ($cc) = @_;

    my $ccode = <<EOT;
#include <stdio.h>
int main(int argc, char* argv[]) {
    printf("Hello, world!\\n");
    return 0;
}
EOT

    return compile("$cc", $ccode, $cc, sub {
        my ($compiles, $status) = @_;
        return $compiles && $status == 0;
    })
}

my $compiler = undef;

print("Trying to find a C compiler...\n");
foreach my $cc ($ENV{CC}, 'cc', 'gcc', 'clang') {
    next if !defined($cc);
    next if !tryCompiler($cc);
    $compiler = $cc;
    last;
}

die("Can't rely on compiler") if not $compiler;

print("Trying to guess how to compile C99\n");
my $c99code = <<EOT;
#if defined(__STDC__)
# if defined(__STDC_VERSION__)
#  if __STDC_VERSION__ >= 199901L
#   define OK 0
#  else
#   error "__STDC_VERSION__ is too old"
#   define OK -1
#  endif
# else
#  error "__STDC_VERSION__ not defined"
#   define OK -1
# endif
#else
# error "__STDC__ not defined"
#   define OK -1
#endif
int main(int argc, char* argv[]) {
    return OK;
}
EOT
my $c99switch = "";
foreach my $flag ("", "-std=c99", "-std=gnu99", "--std=c99", "--std=gnu99") {
    if(compile($flag, $c99code, "$compiler $flag", sub {
        my ($compiles, $status) = @_;
        return $compiles && ($status == 0);
    })) {
        $c99switch = $flag;
        last;
    }
}

my $cc = "$compiler $c99switch";


##################################################################
# library features
##################################################################


my $pledgeCode = <<EOT;
#include <unistd.h>
int main(int argc, char* argv[]) {
    pledge(NULL, NULL);
    return 0;
}
EOT
compile("for pledge", $pledgeCode, $cc, sub {
    my ($compiles, $status) = @_;
    $defines{HAVE_PLEDGE} = ($compiles && $status == 0) ? 1 : 0;
});

my $unveilCode = <<EOT;
#include <unistd.h>
int main(int argc, char* argv[]) {
    unveil(NULL, NULL);
    return 0;
}
EOT
compile("for unveil", $unveilCode, $cc, sub {
    my ($compiles, $status) = @_;
    $defines{HAVE_UNVEIL} = ($compiles && $status == 0) ? 1 : 0;
});

my $errhCode = <<EOT;
#include <err.h>
#include <errno.h>
int main(int argc, char* argv[]) {
    errno = EAGAIN;
    warn("Warning %d", 42);
    err(126, "Message %d", 126);
    return 0;
}
EOT
compile("for err() and warn()", $errhCode, $cc, sub {
    my ($compiles, $status) = @_;
    my $include = "#include <err.h>";
    my $fake = <<EOT;
#define err(eval, fmt, ...) do{\\
if(errno) {\\
fprintf(stderr, "ERR: " fmt ": %d\n",##__VA_ARGS__, errno);\\
exit((eval));\\
}\\
}while(0)
#define warn(fmt, ...) do{\\
fprintf(stderr, "WARN: " fmt ": %d\n",##__VA_ARGS__, errno);\\
}while(0)
EOT
    $defines{ERRH} = ($compiles && $status == 126) ? $include : $fake;
});


my $treeCode = <<EOT;
#include <sys/tree.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
    RB_ENTRY(node) entry;
    int i;
};

int intcmp(struct node *, struct node *);
void    print_tree(struct node *);

int
intcmp(struct node *e1, struct node *e2)
{
    return (e1->i < e2->i ? -1 : e1->i > e2->i);
}

RB_HEAD(inttree, node) head = RB_INITIALIZER(&head);
RB_PROTOTYPE(inttree, node, entry, intcmp)
RB_GENERATE(inttree, node, entry, intcmp)

int testdata[] = {
    20, 16, 17, 13, 3, 6, 1, 8, 2, 4, 10, 19, 5, 9, 12, 15, 18,
    7, 11, 14
};

void
print_tree(struct node *n)
{
    struct node *left, *right;

    if (n == NULL) {
        printf("nil");
        return;
    }
    left = RB_LEFT(n, entry);
    right = RB_RIGHT(n, entry);
    if (left == NULL && right == NULL)
        printf("%d", n->i);
    else {
        printf("%d(", n->i);
        print_tree(left);
        printf(",");
        print_tree(right);
        printf(")");
    }
}

int
main(void)
{
    int i;
    struct node *n;

    for (i = 0; i < sizeof(testdata) / sizeof(testdata[0]); i++) {
        if ((n = malloc(sizeof(struct node))) == NULL)
            exit(2);
        n->i = testdata[i];
        RB_INSERT(inttree, &head, n);
    }

    RB_FOREACH(n, inttree, &head) {
        printf("%d\\n", n->i);
    }
    print_tree(RB_ROOT(&head));
    printf("\\n");
    return (0);
}
EOT

my $systreehinclude = "#include <sys/tree.h>";
if(!defined($libbsdroot)) {
    if(!compile("for native sys/tree.h", $treeCode, "$cc $cflags", sub {
        my ($compiles, $status) = @_;
    
        return $compiles && ($status == 0);
    }, $ldflags)) {
        $treeCode =~ s|#include <sys/tree\.h>|#include <bsd/sys/tree.h>|;
        if(compile("for bsd/sys/tree.h with libbsd", $treeCode, "$cc", sub {
            my ($compiles, $status) = @_;
    
            return $compiles && ($status == 0);
        }, "-lbsd")) {
            $systreehinclude = "#include <bsd/sys/tree.h>";
            $ldflags .= " -lbsd";
        } else {
            die("Can't find sys/tree.h");
        }
    }
} else {
    $treeCode =~ s|#include <sys/tree\.h>|#include <bsd/sys/tree.h>|;
    if(!compile("for sys/tree.h with \$libbsdroot", $treeCode, "$cc $cflags -isystem$libbsdroot/include -DLIBBSD_OVERLAY ", sub {
        my ($compiles, $status) = @_;
    
        return $compiles && ($status == 0);
    }, $ldflags." -L$libbsdroot/lib -lbsd ")) {
        die("Can't find sys/tree.h");
    } else {
        $systreehinclude = "#include <bsd/sys/tree.h>";
        $cflags .= " -DLIBBSD_OVERLAY -isystem$libbsdroot/include";
        $ldflags .= " -L$libbsdroot -lbsd";
    }
}


# TODO prompt user to install libbsd
# TODO add flag to specify where libbsd is located



####################################################################
# write output
####################################################################

my $systempaths = <<EOT;
#define ETC_CONF "$etc_conf"
#define HOME_CONF "$home_conf"
EOT

open A, ">config.h";
print A <<EOT;
// This file was generated by ./configure.pl
// Feel free to modify the defaults
#ifndef CONFIG_H
#define CONFIG_H

$defines{ERRH}

$systreehinclude

#define HAVE_PLEDGE $defines{HAVE_PLEDGE}
#define HAVE_UNVEIL $defines{HAVE_UNVEIL}

$systempaths

#endif
EOT
close(A);

open A, ">Makefile.vars";
print A <<EOT;
# This file was generated by ./configure.pl
# Feel free to modify the defaults
CC = $compiler
CFLAGS = $c99switch $cflags
LDFLAGS = $ldflags
PREFIX = $prefix
EOT
close(A);
