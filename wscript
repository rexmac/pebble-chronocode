from waflib import Logs

#VERSION = 'myversion'
top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')
    #ctx.add_option('-l', '--lang', dest='lang', default=False, action='store_true', help='Language of watchface')

def configure(ctx):
    ctx.load('pebble_sdk')

def build(ctx):
    ctx.load('pebble_sdk')

    #if (ctx.options.lang):
    #    Logs.pprint('CYAN', 'Watchface language: %s' % ctx.options.lang)
    #else:
    #    Logs.pprint('CYAN', 'Watchface language: %s (default)' % 'en_US')
    #ctx(
    #    features = 'subst',
    #    source = 'src/wordsquare.c,',
    #    target = 'pebble-src/wordsquare.c',
    #    LANG = ctx.options.lang)

    ctx.pbl_program(source=ctx.path.ant_glob('pebble-src/**/*.c'),
                    target='pebble-app.elf')

    ctx.pbl_bundle(elf='pebble-app.elf',
                   js=ctx.path.ant_glob('pebble-src/js/**/*.js'))
