#!/usr/bin/env python

import sys
import struct
import json
import copy


class Compiler:
    def __init__(self):
        self.programs = []

        self.channels = {}
        self.channels_l = []

        self.bars = {}
        self.bars_l = []

        self.notes = []

        self.data = {}

        self.raw_notes = ""
        self.raw_bars = ""
        self.raw_channels = ""
        self.raw_progs = ""
        self.output = ""

        self.note_format = "BBBB"
        self.bar_format = "HH"



        self.mods = {
            "rev": self.rev,
            "inv": self.inv,
            "exp": self.exp,
            "lsh": self.lsh,
        }

        self.compile_steps = [self.make_bars, self.make_mods, self.make_programs];

    def compile(self, filename):
        print "Compiling {0}".format(filename)

        error = None

        with open(filename, 'r') as fh:
            self.data = json.load(fh)

            for step in self.compile_steps:
                error = step()
                if error:
                    return error

        return error

    def make_bars(self):
        incoming_bars = self.data["bars"]
        print "Found {0} bars".format(len(incoming_bars))

        for bar in incoming_bars:
            notes = bar["notes"]
            bar_data = {
                "name": bar["name"],
                "count": len(notes),
                "start": len(self.notes),
                "index": len(self.bars)
            }

            for note in notes:
                self.add_note(note)

            self.bars[bar_data["name"]] = bar_data
            self.bars_l.append(bar_data)

        return None

    def add_note(self, note):
        mask = 0
        str_mask = note[3]

        for i in range(0, 8):
            if str_mask[i] == "1":
                mask += 2**i

        self.notes.append([note[0] + 5, note[1], note[2], mask])

    def make_mods(self):
        incoming_mods = self.data["mods"]
        print "Found {0} mods".format(len(incoming_mods))

        for mod in incoming_mods:
            error = self.apply_mod(mod)
            if error:
                return error

        return None

    def apply_mod(self, mod):
        mod_str = mod["mod"]
        (mod_name, mod_args) = mod_str.split("(")
        mod_args = mod_args.rstrip(")").split(",")
        mod_source = self.bars[mod_args[0]]
        mod_args = mod_args[1:]

        notes = self.map_mod(mod_name, mod_source, mod_args)

        bar_data = {
            "name": mod["name"],
            "count": len(notes),
            "start": len(self.notes),
            "index": len(self.bars)
        }

        self.bars[bar_data["name"]] = bar_data
        self.bars_l.append(bar_data)

        self.notes.extend(notes)

        return None

    def map_mod(self, name, source, args):
        if name in self.mods:
            return self.mods[name](source, args)
        else:
            return []

        '''
        inv(bar) (flip bits on all of the notes)
rev(bar) (reverse notes of the sourece bar)
exp(bar, fac) (expand the bar by the factor)
lsh(bar, fac) (left shift the bits by factor)
'''

    def rev(self, source, args):
        count = source["count"]
        start = source["start"]
        return self.notes[start:start+count][::-1]

    def inv(self, source, args):
        out = []
        count = source["count"]
        start = source["start"]

        for item in self.notes[start:start+count]:
            i2 = copy.copy(item)
            i2[3] = (255 & ~i2[3])
            out.append(i2)

        return out

    def exp(self, source, args):
        out = []
        count = source["count"]
        start = source["start"]

        for item in self.notes[start:start+count]:
            i2 = copy.copy(item)
            i2[0] += args[0]
            out.append(i2)

        return out

    def lsh(self, source, args):
        out = []
        count = source["count"]
        start = source["start"]

        for item in self.notes[start:start+count]:
            i2 = copy.copy(item)
            i2[3] = self.llsh(i2[3], args[0])
            out.append(i2)

        return out

    def llsh(self, value, amount):
        if amount == 0:
            return value
        elif amount > 0:
            intermediary = value << amount
            ret = intermediary & 255
            ret &= ((intermediary >> 8) & 255)
            return ret
        else:
            intermediary = value
            while amount < 0:
                inc = intermediary & 1
                intermediary = (intermediary >> 1) & (inc << 7)
                amount += 1
            return intermediary

    def make_programs(self):
        incoming_progs = self.data["programs"]
        print "Found {0} programs".format(len(incoming_progs))

        for prog in incoming_progs:
            parsed_prog = {}
            channels = prog["channels"]
            leng = len(channels)
            if leng == 0:
                continue
            parsed_prog["count"] = leng

            chan_idx = []

            for channel in channels:
                key = "".join(channel)
                if key not in self.channels:
                    chan = self.make_channel(channel)
                    self.channels[key] = chan
                    self.channels_l.append(chan)

                chan_idx.append(self.channels[key]["index"])

            parsed_prog["channels"] = chan_idx

            self.programs.append(parsed_prog)

        return None

    def make_channel(self, parsed):
        chan = {
            "index": len(self.channels),
            "count": len(parsed)
        }

        bar_idx = []
        for bar in parsed:
            bar_idx.append(self.bars[bar]["index"])

        chan["bars"] = bar_idx

        return chan

    def write(self, filename):

        with open(filename, 'w') as fh:
            self.write_header(fh)

            self.generate_raw_notes()
            self.write_raw_notes(fh)

            self.generate_raw_bars()
            self.write_raw_bars(fh)

            self.generate_raw_channels()
            self.write_raw_channels(fh)

            self.generate_raw_programs()
            self.write_raw_programs(fh)

        return None

    def generate_raw_notes(self):
        for note in self.notes:
            self.raw_notes += struct.pack(self.note_format, note[0], note[1], note[2], note[3])

    def generate_raw_bars(self):
        for bar in self.bars_l:
            self.raw_bars += struct.pack(self.bar_format, bar["count"], bar["start"])

        return None

    def generate_raw_channels(self):
        for chan in self.channels_l:
            print chan
            total = chan["count"] + 1
            fmt = "H" * total
            self.raw_channels += struct.pack(fmt, total - 1, *chan["bars"])

        return None

    def generate_raw_programs(self):
        for prog in self.programs:
            print prog
            total = prog["count"] + 1;
            fmt = "H" * total
            self.raw_progs += struct.pack(fmt, total - 1, *prog["channels"])

        return None

    def write_header(self, fh):
        fh.write("static int PROGRAM_COUNT = {0};\n".format(len(self.programs)))
        fh.write("static int CHANNEL_COUNT = {0};\n".format(len(self.channels)))
        fh.write("static int BAR_COUNT = {0};\n\n".format(len(self.bars)))
        return None

    def write_raw_notes(self, fh):
        fh.write("static unsigned char NOTES[] = {\n")
        self.output_raw(fh, self.raw_notes)
        fh.write("};\n\n")
        return None

    def write_raw_bars(self, fh):
        fh.write("static unsigned char BARS[] = {\n")
        self.output_raw(fh, self.raw_bars)
        fh.write("};\n\n")
        return None

    def write_raw_channels(self, fh):
        fh.write("static unsigned char CHANNELS[] = {\n")
        self.output_raw(fh, self.raw_channels)
        fh.write("};\n\n")

    def write_raw_programs(self, fh):
        fh.write("static unsigned char PROGRAMS[] = {\n")
        self.output_raw(fh, self.raw_progs)
        fh.write("};\n\n")
        return None

    def output_raw(self, fh, data):
        fh.write(",\n".join(self.commafy(self.sliceify(self.hexify(data), 16))))
        fh.write("\n")

    def commafy(self, iterable):
        return map(lambda x: ", ".join(x), iterable)

    def hexify(self, raw):
        return map(lambda x: "{0:#04x}".format(ord(x)), raw)

    def sliceify(self, iterable, slice_size):
        for i in xrange(0, len(iterable), slice_size):
            yield iterable[i:i+slice_size]


if __name__ == "__main__":

    if len(sys.argv) < 2:
        print "Usage:\n\t{0} <input> <output>\n\t\tinput: bthd file\n\t\toutput: optional .h file (default raw.h)".format(sys.argv[0])
        exit()

    compiler = Compiler()

    status = compiler.compile(sys.argv[1])
    if status:
        print "error: {0}".format(status)
        exit()

    out_file = "raw.h"

    if len(sys.argv) > 2:
        out_file = sys.argv[2]

    compiler.write(out_file)
