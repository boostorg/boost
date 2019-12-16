from __future__ import absolute_import, print_function
import sys, argparse, traceback, os, copy
if __name__ == '__main__' and __package__ is None:
    sys.path.append( os.path.dirname( os.path.dirname( os.path.abspath(__file__) ) ) )
from pcpp.preprocessor import Preprocessor, OutputDirective, Action

version='1.21'

__all__ = []

class FileAction(argparse.Action):
    def __init__(self, option_strings, dest, **kwargs):
        super(FileAction, self).__init__(option_strings, dest, **kwargs)
        
    def __call__(self, parser, namespace, values, option_string=None):
        if getattr(namespace, self.dest)[0] == sys.stdin:
            items = []
        else:
            items = copy.copy(getattr(namespace, self.dest))
        items += [argparse.FileType('rt')(value) for value in values]
        setattr(namespace, self.dest, items)

class CmdPreprocessor(Preprocessor):
    def __init__(self, argv):
        if len(argv) < 2:
            argv = [argv[0], '--help']
        argp = argparse.ArgumentParser(prog='pcpp',
            description=
    '''A pure universal Python C (pre-)preprocessor implementation very useful for
    pre-preprocessing header only C++ libraries into single file includes and
    other such build or packaging stage malarky.''',
            epilog=
    '''Note that so pcpp can stand in for other preprocessor tooling, it
    ignores any arguments it does not understand.''')
        argp.add_argument('inputs', metavar = 'input', default = [sys.stdin], nargs = '*', action = FileAction, help = 'Files to preprocess (use \'-\' for stdin)')
        argp.add_argument('-o', dest = 'output', metavar = 'path', type = argparse.FileType('wt'), default=sys.stdout, nargs = '?', help = 'Output to a file instead of stdout')
        argp.add_argument('-D', dest = 'defines', metavar = 'macro[=val]', nargs = 1, action = 'append', help = 'Predefine name as a macro [with value]')
        argp.add_argument('-U', dest = 'undefines', metavar = 'macro', nargs = 1, action = 'append', help = 'Pre-undefine name as a macro')
        argp.add_argument('-N', dest = 'nevers', metavar = 'macro', nargs = 1, action = 'append', help = 'Never define name as a macro, even if defined during the preprocessing.')
        argp.add_argument('-I', dest = 'includes', metavar = 'path', nargs = 1, action = 'append', help = "Path to search for unfound #include's")
        #argp.add_argument('--passthru', dest = 'passthru', action = 'store_true', help = 'Pass through everything unexecuted except for #include and include guards (which need to be the first thing in an include file')
        argp.add_argument('--passthru-defines', dest = 'passthru_defines', action = 'store_true', help = 'Pass through but still execute #defines and #undefs if not always removed by preprocessor logic')
        argp.add_argument('--passthru-unfound-includes', dest = 'passthru_unfound_includes', action = 'store_true', help = 'Pass through #includes not found without execution')
        argp.add_argument('--passthru-unknown-exprs', dest = 'passthru_undefined_exprs', action = 'store_true', help = 'Unknown macros in expressions cause preprocessor logic to be passed through instead of executed by treating unknown macros as 0L')
        argp.add_argument('--passthru-comments', dest = 'passthru_comments', action = 'store_true', help = 'Pass through comments unmodified')
        argp.add_argument('--passthru-magic-macros', dest = 'passthru_magic_macros', action = 'store_true', help = 'Pass through double underscore magic macros unmodified')
        argp.add_argument('--disable-auto-pragma-once', dest = 'auto_pragma_once_disabled', action = 'store_true', default = False, help = 'Disable the heuristics which auto apply #pragma once to #include files wholly wrapped in an obvious include guard macro')
        argp.add_argument('--line-directive', dest = 'line_directive', metavar = 'form', default = '#line', nargs = '?', help = "Form of line directive to use, defaults to #line, specify nothing to disable output of line directives")
        argp.add_argument('--debug', dest = 'debug', action = 'store_true', help = 'Generate a pcpp_debug.log file logging execution')
        argp.add_argument('--time', dest = 'time', action = 'store_true', help = 'Print the time it took to #include each file')
        argp.add_argument('--filetimes', dest = 'filetimes', metavar = 'path', type = argparse.FileType('wt'), default=None, nargs = '?', help = 'Write CSV file with time spent inside each included file, inclusive and exclusive')
        argp.add_argument('--compress', dest = 'compress', action = 'store_true', help = 'Make output as small as possible')
        argp.add_argument('--version', action='version', version='pcpp ' + version)
        args = argp.parse_known_args(argv[1:])
        #print(args)
        for arg in args[1]:
            print("NOTE: Argument %s not known, ignoring!" % arg, file = sys.stderr)

        self.args = args[0]
        super(CmdPreprocessor, self).__init__()
        
        # Override Preprocessor instance variables
        self.define("__PCPP_VERSION__ " + version)
        self.define("__PCPP_ALWAYS_FALSE__ 0")
        self.define("__PCPP_ALWAYS_TRUE__ 1")
        if self.args.debug:
            self.debugout = open("pcpp_debug.log", "wt")
        self.auto_pragma_once_enabled = not self.args.auto_pragma_once_disabled
        self.line_directive = self.args.line_directive
        self.compress = 2 if self.args.compress else 0
        if self.args.passthru_magic_macros:
            self.undef('__DATE__')
            self.undef('__TIME__')
            self.expand_linemacro = False
            self.expand_filemacro = False
            self.expand_countermacro = False
        
        # My own instance variables
        self.bypass_ifpassthru = False
        self.potential_include_guard = None

        if self.args.defines:
            self.args.defines = [x[0] for x in self.args.defines]
            for d in self.args.defines:
                if '=' not in d:
                    d += '=1'
                d = d.replace('=', ' ', 1)
                self.define(d)
        if self.args.undefines:
            self.args.undefines = [x[0] for x in self.args.undefines]
            for d in self.args.undefines:
                self.undef(d)
        if self.args.nevers:
            self.args.nevers = [x[0] for x in self.args.nevers]
        if self.args.includes:
            self.args.includes = [x[0] for x in self.args.includes]
            for d in self.args.includes:
                self.add_path(d)

        try:
            if len(self.args.inputs) == 1:
                self.parse(self.args.inputs[0])
            else:
                input = ''
                for i in self.args.inputs:
                    input += '#include "' + i.name + '"\n'
                self.parse(input)
            self.write(self.args.output)
        except:
            print(traceback.print_exc(10), file = sys.stderr)
            print("\nINTERNAL PREPROCESSOR ERROR AT AROUND %s:%d, FATALLY EXITING NOW\n"
                % (self.lastdirective.source, self.lastdirective.lineno), file = sys.stderr)
            sys.exit(-99)
        finally:
            for i in self.args.inputs:
                i.close()
            self.args.output.close()
        
        if self.args.time:
            print("\nTime report:")
            print("============")
            for n in range(0, len(self.include_times)):
                if n == 0:
                    print("top level: %f seconds" % self.include_times[n].elapsed)
                elif self.include_times[n].depth == 1:
                    print("\n %s: %f seconds (%f%%)" % (self.include_times[n].included_path, self.include_times[n].elapsed, 100 * self.include_times[n].elapsed / self.include_times[0].elapsed))
                else:
                    print("%s%s: %f seconds" % (' ' * self.include_times[n].depth, self.include_times[n].included_path, self.include_times[n].elapsed))
            print("\nPragma once files (including heuristically applied):")
            print("====================================================")
            for i in self.include_once:
                print(" ", i)
            print()
        if self.args.filetimes:
            print('"Total seconds","Self seconds","File size","File path"', file = self.args.filetimes)
            filetimes = {}
            currentfiles = []
            for n in range(0, len(self.include_times)):
                while self.include_times[n].depth < len(currentfiles):
                    currentfiles.pop()
                if self.include_times[n].depth > len(currentfiles) - 1:
                    currentfiles.append(self.include_times[n].included_abspath)
                #print()
                #for path in currentfiles:
                #    print("currentfiles =", path)
                path = currentfiles[-1]
                if path in filetimes:
                    filetimes[path][0] += self.include_times[n].elapsed
                    filetimes[path][1] += self.include_times[n].elapsed
                else:
                    filetimes[path] = [self.include_times[n].elapsed, self.include_times[n].elapsed]
                if self.include_times[n].elapsed > 0 and len(currentfiles) > 1:
                    #print("Removing child %f from parent %s = %f" % (self.include_times[n].elapsed, currentfiles[-2], filetimes[currentfiles[-2]]))
                    filetimes[currentfiles[-2]][1] -= self.include_times[n].elapsed
            filetimes = [(v[0],v[1],k) for k,v in filetimes.items()]
            filetimes.sort(reverse=True)
            for t,s,p in filetimes:
                print(('%f,%f,%d,"%s"' % (t, s, os.stat(p).st_size, p)), file = self.args.filetimes)
    def on_include_not_found(self,is_system_include,curdir,includepath):
        if self.args.passthru_unfound_includes:
            raise OutputDirective(Action.IgnoreAndPassThrough)
        return super(CmdPreprocessor, self).on_include_not_found(is_system_include,curdir,includepath)

    def on_unknown_macro_in_defined_expr(self,tok):
        if self.args.undefines:
            if tok.value in self.args.undefines:
                return False
        if self.args.passthru_undefined_exprs:
            return None  # Pass through as expanded as possible
        return super(CmdPreprocessor, self).on_unknown_macro_in_defined_expr(tok)
        
    def on_unknown_macro_in_expr(self,tok):
        if self.args.undefines:
            if tok.value in self.args.undefines:
                return super(CmdPreprocessor, self).on_unknown_macro_in_expr(tok)
        if self.args.passthru_undefined_exprs:
            return None  # Pass through as expanded as possible
        return super(CmdPreprocessor, self).on_unknown_macro_in_expr(tok)
        
    def on_directive_handle(self,directive,toks,ifpassthru,precedingtoks):
        if ifpassthru:
            if directive.value == 'if' or directive.value == 'elif' or directive == 'else' or directive.value == 'endif':
                self.bypass_ifpassthru = len([tok for tok in toks if tok.value == '__PCPP_ALWAYS_FALSE__' or tok.value == '__PCPP_ALWAYS_TRUE__']) > 0
            if not self.bypass_ifpassthru and (directive.value == 'define' or directive.value == 'undef'):
                if toks[0].value != self.potential_include_guard:
                    raise OutputDirective(Action.IgnoreAndPassThrough)  # Don't execute anything with effects when inside an #if expr with undefined macro
        if (directive.value == 'define' or directive.value == 'undef') and self.args.nevers:
            if toks[0].value in self.args.nevers:
                raise OutputDirective(Action.IgnoreAndPassThrough)
        if self.args.passthru_defines:
            super(CmdPreprocessor, self).on_directive_handle(directive,toks,ifpassthru,precedingtoks)
            return None  # Pass through where possible
        return super(CmdPreprocessor, self).on_directive_handle(directive,toks,ifpassthru,precedingtoks)

    def on_directive_unknown(self,directive,toks,ifpassthru,precedingtoks):
        if ifpassthru:
            return None  # Pass through
        return super(CmdPreprocessor, self).on_directive_unknown(directive,toks,ifpassthru,precedingtoks)

    def on_potential_include_guard(self,macro):
        self.potential_include_guard = macro
        return super(CmdPreprocessor, self).on_potential_include_guard(macro)

    def on_comment(self,tok):
        if self.args.passthru_comments:
            return True  # Pass through
        return super(CmdPreprocessor, self).on_comment(tok)

def main():
    p = CmdPreprocessor(sys.argv)
    sys.exit(p.return_code)
        
if __name__ == "__main__":
    p = CmdPreprocessor(sys.argv)
    sys.exit(p.return_code)
