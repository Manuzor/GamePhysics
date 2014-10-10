from os import path, makedirs, stat
from shutil import copy2
from argparse import ArgumentParser
from imp import load_source
from ntpath import basename

SupportedArchitectureOptions = [ "32", "64" ]
SupportedConfigurationOptions = [ "Release", "Debug" ]

parser = ArgumentParser(description="Copy dependencies, specified in a config file, to the given directory.")
parser.add_argument("-n", "--DryRun",
                    action="store_true",
                    help="Simulates what would be done, but does not actually do anything other than printing. Will respect -v.")
parser.add_argument("-v", "--VerbosityLevel",
                    type=int,
                    default=1,
                    help="Specifies how verbose the output will be. If 0, no output is printed. Default is 1.")
parser.add_argument("-f", "--Force",
                    action="store_true",
                    help="Force copying of the files, even if they're older or up to date.")
parser.add_argument("OutDir",
                    type=lambda s: s.strip(),
                    help="The directory where the dependencies should be copied to.")
parser.add_argument("Configuration",
                    type=lambda s: s.strip(),
                    help="The configuration of the build.")
parser.add_argument("Architecture",
                    type=lambda s: s.strip(),
                    help="The platform architecture of the build.")
parser.add_argument("ConfigFile",
                    type=lambda s: s.strip(),
                    help="The file containing the dependency descriptions.")

args = parser.parse_args()


def Print(ToPrint):
    if args.VerbosityLevel > 0:
        print("  {0}".format(ToPrint))

# Creates the given directory, if it does not exist already.
def CreateDirectory(Directory):
    if not path.exists(Directory):
        makedirs(Directory)

def CopyIfNewer(Source, Target):
    Source = path.normpath(Source)
    Target = path.normpath(Target)
    if not args.Force and path.isfile(Target):
        ModificationTimeDelta = stat(Source).st_mtime - stat(Target).st_mtime
        CopyThreshold = 1 # second

        if ModificationTimeDelta < CopyThreshold:
            Print("Already up to date: {0}".format(Target))
            return

    if args.DryRun:
        if not path.exists(Source):
            Print("Warning: File could not be found: {0}".format(Source))
            return
        Print("Would copy: {0} -> {1}".format(Source, Target))
        return
    try:
        Print("Copying: {0} -> {1}".format(Source, Target))
        copy2(Source, Target)
    except FileNotFoundError as err:
        Print("File could not be found: {0}".format(Source))

def Main():
    print("Copying dependencies...".format(args.OutDir))
    if args.VerbosityLevel > 1:
        Print(args)

    CreateDirectory(args.OutDir)

    Config = load_source("Config", path.abspath(args.ConfigFile))
    dependencies = Config.GetDependencies(args.Configuration.strip(), args.Architecture.strip())
    for dep in dependencies:
        FileName = basename(dep)
        Source = dep
        Target = path.join(args.OutDir, FileName)
        CopyIfNewer(Source, Target)

if __name__ == "__main__":
    Main()
