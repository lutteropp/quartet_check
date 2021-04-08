#ifndef GENESIS_PLACEMENT_SAMPLE_SET_H_
#define GENESIS_PLACEMENT_SAMPLE_SET_H_

/*
    Genesis - A toolkit for working with phylogenetic data.
    Copyright (C) 2014-2018 Lucas Czech and HITS gGmbH

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
 * @ingroup placement
 */

#include "genesis/placement/sample.hpp"

#include <string>
#include <vector>

namespace genesis {
namespace placement {

// =================================================================================================
//     SampleSet
// =================================================================================================

/**
 * @brief Store a set of Sample%s with associated names.
 *
 * The elements in this set are stored as a NamedSample. They are stored in the sequence in which
 * they are added to the set, and can be accessed via an index.
 */
class SampleSet
{
public:

    // -------------------------------------------------------------------------
    //     Typedefs
    // -------------------------------------------------------------------------

    /**
     * @brief Store a Sample and a name for it.
     *
     * Access to the Sample and name is explicit via the public member variables.
     * Additionally, an implicit cast to Sample is available to avoid tedious code.
     */
    struct NamedSample
    {
        std::string name;
        Sample      sample;

        operator Sample&()
        {
            return sample;
        }

        operator Sample const&() const
        {
            return sample;
        }
    };

    typedef std::vector<NamedSample>::iterator       iterator;
    typedef std::vector<NamedSample>::const_iterator const_iterator;

    // -------------------------------------------------------------------------
    //     Constructors and Rule of Five
    // -------------------------------------------------------------------------

    SampleSet()  = default;
    ~SampleSet() = default;

    SampleSet( SampleSet const& ) = default;
    SampleSet( SampleSet&& )      = default;

    SampleSet& operator= ( SampleSet const& ) = default;
    SampleSet& operator= ( SampleSet&& )      = default;

    void swap( SampleSet& other )
    {
        using std::swap;
        swap( smps_, other.smps_ );
    }

    // -------------------------------------------------------------------------
    //     Modifiers
    // -------------------------------------------------------------------------

    /**
     * @brief Add a Sample to the SampleSet.
     *
     * The Sample is copied. The name is set to empty string.
     */
    void add( Sample const& smp )
    {
        smps_.push_back( { "", smp } );
    }

    /**
     * @brief Add a Sample with a name to the SampleSet.
     *
     * The Sample is copied.
     */
    void add( Sample const& smp, std::string const& name )
    {
        smps_.push_back( { name, smp } );
    }

    /**
     * @brief Add a Sample to the SampleSet.
     *
     * The Sample is moved. The name is set to empty string.
     */
    void add( Sample&& smp )
    {
        smps_.push_back( { "", std::move(smp) } );
    }

    /**
     * @brief Add a Sample with a name to the SampleSet.
     *
     * The Sample is moved.
     */
    void add( Sample&& smp, std::string const& name )
    {
        smps_.push_back( { name, std::move(smp) } );
    }

    /**
     * @brief Remove the Sample at a certain index position.
     *
     * As this function moves Sample%s in the container around, all iterators and pointers to
     * the elements of this SampleSet are considered to be invalidated.
     */
    void remove_at( size_t index )
    {
        smps_.erase( smps_.begin() + index );
    }

    /**
     * @brief Delete all Sample%s in this SampleSet.
     */
    void clear()
    {
        smps_.clear();
    }

    // -------------------------------------------------------------------------
    //     Accessors
    // -------------------------------------------------------------------------

    iterator begin()
    {
        return smps_.begin();
    }

    const_iterator begin() const
    {
        return smps_.cbegin();
    }

    iterator end()
    {
        return smps_.end();
    }

    const_iterator end() const
    {
        return smps_.cend();
    }

    /**
     * @brief Get the NamedSample at a certain index position.
     */
    NamedSample& at ( size_t index )
    {
        return smps_.at(index);
    }

    /**
     * @brief Get the NamedSample at a certain index position.
     */
    NamedSample const& at ( size_t index ) const
    {
        return smps_.at(index);
    }

    /**
     * @brief Get the NamedSample at a certain index position.
     */
    NamedSample& operator [] ( size_t index )
    {
        return smps_[index];
    }

    /**
     * @brief Get the NamedSample at a certain index position.
     */
    NamedSample const& operator [] ( size_t index ) const
    {
        return smps_[index];
    }

    /**
     * @brief Return whether the SampleSet is empty.
     */
    bool  empty() const
    {
        return smps_.empty();
    }

    /**
     * @brief Return the size of the SampleSet, i.e., the number of Sample%s.
     */
    size_t size() const
    {
        return smps_.size();
    }

    // -------------------------------------------------------------------------
    //     Data Members
    // -------------------------------------------------------------------------

private:

    std::vector<NamedSample> smps_;

};

} // namespace placement
} // namespace genesis

#endif // include guard
