# $Id$
#
# Copyright (C) 2006  Kipp C. Cannon
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#
# =============================================================================
#
#                                   Preamble
#
# =============================================================================
#

"""
Definitions of type strings found in LIGO Light Weight XML files.
"""

__author__ = "Kipp Cannon <kipp@gravity.phys.uwm.edu>"
__date__ = "$Date$"[7:-2]
__version__ = "$Revision$"[11:-2]


#
# =============================================================================
#
#                               Type Information
#
# =============================================================================
#

StringTypes = ["char_s", "ilwd:char", "ilwd:char_u", "lstring", "string"]
IntTypes = ["int_2s", "int_2u", "int_4s", "int_4u", "int_8s", "int_8u", "int"]
FloatTypes = ["real_4", "real_8", "float", "double"]

Types = StringTypes + IntTypes + FloatTypes

ToFormat = {
	"int_2s": "%d",
	"int_2u": "%u",
	"int_4s": "%d",
	"int_4u": "%u",
	"int_8s": "%d",
	"int_8u": "%u",
	"int": "%d",
	"real_4": "%.7g",
	"real_8": "%.16g",
	"float": "%.7g",
	"double": "%.16g"
}

ToNumArrayType = {
	"int_2s": "Int16",
	"int_2u": "UInt16",
	"int_4s": "Int32",
	"int_4u": "UInt32",
	"int_8s": "Int64",
	"int_8u": "UInt64",
	"int": "Int32",
	"real_4": "Float32",
	"real_8": "Float64",
	"float": "Float64",
	"double": "Float64"
}

FromNumArrayType = {
	"Int16": "int_2s",
	"UInt16": "int_2u",
	"Int32": "int_4s",
	"UInt32": "int_4u",
	"Int64": "int_8s",
	"UInt64": "int_8u",
	"Float32": "real_4",
	"Float64": "real_8"
}

ToSQLiteType = {
	"char_s": "TEXT",
	"ilwd:char": "TEXT",
	"ilwd:char_u": "TEXT",
	"lstring": "TEXT",
	"string": "TEXT",
	"int_2s": "INTEGER",
	"int_2u": "INTEGER",
	"int_4s": "INTEGER",
	"int_4u": "INTEGER",
	"int_8s": "INTEGER",
	"int_8u": "INTEGER",
	"int": "INTEGER",
	"real_4": "REAL",
	"real_8": "REAL",
	"float": "REAL",
	"double": "REAL"
}
