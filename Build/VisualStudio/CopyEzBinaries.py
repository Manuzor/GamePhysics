#!/bin/env python

from os import path, makedirs, stat
from shutil import copy2
from argparse import ArgumentParser

ArchitectureOptions = [ "32", "64" ]
ConfigurationOptions = [ "Release", "Debug" ]
DefaultTargetFileTypes = ".dll;.pdb"

#####################################################################

parser = ArgumentParser(description="Copy specific ezEngine binaries to a specified directory")
parser.add_argument("-n", "--DryRun",
                    action="store_true",
                    help="Simulates what would be done, but does not actually do anything other than printing.")
parser.add_argument("--TargetFileTypes",
                    default=DefaultTargetFileTypes,
                    help="A semicolon separated list of extension names of the files to copy. Defaults to \"{0}\"".format(DefaultTargetFileTypes))
parser.add_argument("OutDir",
                    help="The directory where the ezEngine binaries should be copied to.")
parser.add_argument("EzEngineDir",
                    help="The root directory of the ezEngine, i.e. the folder in which the Output folder lives.")
parser.add_argument("Configuration",
                    choices=ConfigurationOptions)
parser.add_argument("Architecture",
                    choices=ArchitectureOptions, help="The Architecture of the binaries to use.")
parser.add_argument("ModuleName",
                    nargs="+",
                    help="The names of the ezEngine modules to copy, such as 'ezFoundation' or 'ezCore' etc.")

args = parser.parse_args()

#####################################################################

def Print(ToPrint):
    print("  " + ToPrint)

def CreateDirectoryIfNoExists(Directory):
    if not path.exists(Directory):
        makedirs(Directory)

def CopyIfNewer(Source, Target):
    if path.isfile(Target):
        ModificationTimeDelta = stat(Source).st_mtime - stat(Target).st_mtime
        CopyThreshold = 1 # second

        if ModificationTimeDelta < CopyThreshold:
            Print("Already up to date: {0}".format(Target))
            return

    if args.DryRun:
        if not path.exists(Source):
            Print("error: File could not be found: {0}".format(Source))
            return
        Print("  {0} => {1}".format(Source, Target))
        return
    try:
        Print("  {0} => {1}".format(Source, Target))
        copy2(Source, Target)
    except FileNotFoundError as err:
        Print("File could not be found: {0}".format(Source))

def main():
    OutDir = path.abspath(args.OutDir)
    EzEngineDir = path.abspath(args.EzEngineDir)

    CreateDirectoryIfNoExists(OutDir)

    EzSubFolder = "WinVs2013{config}{arch}".format(config=args.Configuration, arch=args.Architecture)
    EzEngineDir = path.join(EzEngineDir, "Output", "Bin", EzSubFolder)

    Extensions = args.TargetFileTypes.split(";")

    for ModuleName in args.ModuleName:
        for Extension in Extensions:
            FileName = ModuleName + Extension
            Source = path.join(EzEngineDir, FileName)
            Target = path.join(OutDir, FileName)
            CopyIfNewer(Source, Target)

    return

if __name__ == '__main__':
    main()
