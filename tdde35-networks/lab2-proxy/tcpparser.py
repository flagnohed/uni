import re
import os
import sys
BAD_WORD = 'Smiley'
BAD_PLACE = 'Stockholm'

GOOD_WORD = 'Trolly'
GOOD_PLACE = 'Linköping'

CONTENT_TYPE_CHARSET = 'charset=UTF-8'

def replace_word(str):
    replace_word = re.sub(BAD_WORD, GOOD_WORD, str, flags=re.IGNORECASE)
    replace_place = re.sub(BAD_PLACE, GOOD_PLACE, replace_word, flags=re.IGNORECASE)
    temp = re.sub('Trolly.jpg','trolly.jpg',replace_place)
    temp = re.sub('Linköping-spring.jpg','Stockholm-spring.jpg',temp)
    return temp

def change_contentlenght(indata):
    print(f'indata: {repr(indata)}')
    if indata:
        newlen = sys.getsizeof(indata)
        clstart = indata.find('Content-Length')
        t = indata[clstart:]
        r = indata.split(t)[0]
        clend = t.find('\r\n')
        s = t[:clend]
        if len(s) > 0:
            u = indata.split(s)[1]
            out = r+'Content-Length:' + str(newlen) + u
        return out
    return indata

    
    
