/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.class.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 09:22:02 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/06 18:32:06 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __BOARD_CLASS_HPP
# define __BOARD_CLASS_HPP

# include "libs.hpp"

class Board {
private:
	size_t											size_;		// length of one side of the board
	std::map<std::pair<short, short>, short>		pvals_;		// 2D map address to value
	std::map<short, std::pair<short, short>>		vvals_;		// 2D map value to address
	int												cost_;		// how many steps taken so far
	float											distance_;	// sum of measures (how far from solution)
	std::vector<Board>								children_;	// next states
	Board											*parent_;	// pointer to previous state

	float	solveMetrics_(Board& current, const Board& solution, int metric,
						  const std::pair<short, short>& cur_address);
	void	updateDistance_( int& heuristic );									// distance update while running search
	void	swap_(const std::pair<short, short>& valueAddress,
				  const std::pair<short, short>& zeroAddress,
				  int metric, const Board&  solution);
public:
	Board( void );
	Board( const size_t& size );
	Board( const Board& rhs );

	void	operator=( const Board& rhs );
	bool	operator==( const Board& rhs ) const;		// return true if 2D arrays are identical
	bool	operator!=( const Board& rhs ) const;		// return true if 2D arrays are not identical
	bool	operator<( const Board& rhs ) const;		// return true if

	bool											generateCorrect( void );	// generate solution board
	bool											generateRandom( void );		// generate random initial board
	bool											isUnique( void );			// compare with parents
	size_t											getSize( void ) const;		// get length of side
	std::map<short, std::pair<short, short>>&		getAddressByValue( void );
	const std::map<short, std::pair<short, short>>&	getAddressByValue( void ) const;
	std::map<std::pair<short, short>, short>&		getValueByAddress( void );
	const std::map<std::pair<short, short>, short>&	getValueByAddress( void ) const;
	int												getCost( void ) const;
	float											getDistance( void ) const;
	const Board										*getParent( void ) const;
	std::vector<Board>&								getChildren( void );
	std::pair<int, int>								getNodeAddress(short& val) const;							// find node and return it's 2D address
	short											getNodeValue(const std::pair<int, int>& address) const;		// get value
	void											setChildren( const int& metric, const Board& solution, Board* parent );
	void											setDistance( const int& metric, const Board& solution );	// initial distance setup
};

extern		std::ostream& operator<<( std::ostream& os, const Board& board );
extern		std::istream& operator>>( std::istream& is, Board& board );

#endif
