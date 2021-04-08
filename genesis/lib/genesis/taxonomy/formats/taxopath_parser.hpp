#ifndef GENESIS_TAXONOMY_FORMATS_TAXOPATH_PARSER_H_
#define GENESIS_TAXONOMY_FORMATS_TAXOPATH_PARSER_H_

/*
    Genesis - A toolkit for working with phylogenetic data.
    Copyright (C) 2014-2017 Lucas Czech

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact:
    Lucas Czech <lucas.czech@h-its.org>
    Exelixis Lab, Heidelberg Institute for Theoretical Studies
    Schloss-Wolfsbrunnenweg 35, D-69118 Heidelberg, Germany
*/

/**
 * @brief
 *
 * @file
 * @ingroup taxonomy
 */

#include <string>
#include <vector>

namespace genesis {
namespace taxonomy {

// =================================================================================================
//     Forward Declarations
// =================================================================================================

class Taxon;
class Taxopath;

// =================================================================================================
//     Taxopath Parser
// =================================================================================================

/**
 * @brief Helper class to parse a string containing a taxonomic path string into a Taxopath object.
 *
 * This class bundles the parameters used for parsing a taxonomic path strings and offers functions
 * for the actual parsing. This is needed in order to allow customization of the parsing process,
 * for example in TaxonomyReader. Furthermore, this prevents code duplication in places where the
 * input is a taxonomic path string.
 * The result of the parsing process is a Taxopath object. See there for details.
 *
 * The elements are expected to be char separated, using the value of
 * @link delimiters( std::string const& value ) delimiters()@endlink to separate them.
 * Default is ';'.
 *
 * For example: The input string
 *
 *     Tax_1; Tax_2 ;;Tax_4;
 *
 * is parsed into the Taxopath
 *
 *     [ "Tax_1", "Tax_2", "Tax_2", "Tax_4" ]
 *
 * That is, missing elements are filled up with the preceeding ones - this is a common technique
 * in taxonomic databases, which is useful for unspecified taxa in deeper taxonomies.
 *
 * Furthermore, if the string ends with the delimiter char, this is removed by default. See above
 * for an example of this;
 * see @link remove_trailing_delimiter( bool value ) remove_trailing_delimiter()@endlink
 * to change that behaviour and instead keep this last element.
 * Also, the first taxon in the string cannot be empty. Otherwise an `std::runtime_error` is thrown.
 */
class TaxopathParser
{
public:

    // -------------------------------------------------------------------------
    //     Constructors and Rule of Five
    // -------------------------------------------------------------------------

    TaxopathParser()  = default;
    ~TaxopathParser() = default;

    TaxopathParser( TaxopathParser const& ) = default;
    TaxopathParser( TaxopathParser&& )      = default;

    TaxopathParser& operator= ( TaxopathParser const& ) = default;
    TaxopathParser& operator= ( TaxopathParser&& )      = default;

    // -------------------------------------------------------------------------
    //     Parsing
    // -------------------------------------------------------------------------

    Taxopath from_string( std::string const& taxopath ) const;
    Taxopath operator() ( std::string const& taxopath ) const;

    Taxopath from_taxon(  Taxon const& taxon ) const;
    Taxopath operator() ( Taxon const& taxon ) const;

    // -------------------------------------------------------------------------
    //     Properties
    // -------------------------------------------------------------------------

    TaxopathParser& delimiters( std::string const& value );
    std::string     delimiters() const;

    TaxopathParser& trim_whitespaces( bool value );
    bool            trim_whitespaces() const;

    TaxopathParser& remove_trailing_delimiter( bool value );
    bool            remove_trailing_delimiter() const;

    // -------------------------------------------------------------------------
    //     Data Members
    // -------------------------------------------------------------------------

private:

    std::string delimiters_                = ";";
    bool        trim_whitespaces_          = true;
    bool        remove_trailing_delimiter_ = true;

};

} // namespace taxonomy
} // namespace genesis

#endif // include guard
