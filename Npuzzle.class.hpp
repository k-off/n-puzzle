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
	Board								initialBoard_;
	Board								solutionBoard_;
	std::set<std::pair<float, Board*>>	openStates_;		// states in still in queue
	std::set<uint64_t>					allHashes_;			// hash sums for all previously generated states
	int									maxOpenStates_;		// stores peak amount of openStates_, complexity in size
	int									iterationCounter_;	// stores peak cost, complexity in time
	int									metricId_;			// selected metric
	int									searchTypeID_;		// selected search type

public:
	Npuzzle( void );										// start game in random mode
	Npuzzle( std::istream& infile );						// start game with input file

	void								setMetricId( void );
	void								setSearchType( void );
	bool								solve( void );
	bool								print( Board *solution ) const;
};

#endif
