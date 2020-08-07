/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Npuzzle.class.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 08:57:54 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/01 08:58:01 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __NPUZZLE_CLASS_HPP
# define __NPUZZLE_CLASS_HPP

# include "header.h"
# include "Board.class.hpp"

class Npuzzle {
private:
	Board					initial_;		// initial board gotten from file or generated
	Board					solution_;		// final solution generated at the beginning for comparison
	std::map<float, Board*>	openStates_;	// metric value and pointer to state
	std::set<uint64_t>		allHashes_;		// hash sums for all generated states
	int						maxOpen_;		// stores peak size of openStates_, complexity in size
	int						maxCost_;		// stores peak cost, complexity in time
	int						metricId_;		// selected metric
	int						searchType_;	// selected search type

public:
	Npuzzle( void );										// start game in random mode
	Npuzzle( std::istream& infile );						// start game with input file

	void					setMetricId( void );			// ask user to choose the metric
	void					setSearchType( void );			// ask user to choose the search type
	bool					solve( void );					// main solution loop
	bool					print( Board *solution ) const;	// output result
};

#endif
