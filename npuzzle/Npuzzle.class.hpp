/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Npuzzle.class.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 08:57:54 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/06 18:32:30 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __NPUZZLE_CLASS_HPP
# define __NPUZZLE_CLASS_HPP

# include "libs.hpp"
# include "Board.class.hpp"

class Npuzzle {
private:
	Board								initial_;
	Board								solution_;
	std::set<std::map<std::pair<short, short>, short>>	states_;
	std::set<std::pair<float, Board*>>	open_;
	int									maxStates_;
	int									shortestPath_;
	int									metricId_;
	
public:
	Npuzzle( void );
	Npuzzle( const Board& board );
	Npuzzle( std::istream& infile );
	
	void	setMetricId( void );
	bool	solve( void );
	bool	print( Board *solution ) const;
};

#endif
