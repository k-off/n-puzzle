/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.class.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 09:22:02 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/09 12:59:49 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __BOARD_CLASS_HPP
# define __BOARD_CLASS_HPP

# include "header.h"

struct	address {
	char	x;
	char	y;
};

class Board {
private:
	unsigned char				sideLength_;									// length of one side of the board
	std::vector<unsigned char>	board_;											// the actual map
	int							heuristic_;
	int							cost_;
	float						weight_;										// result of heuristic funciton, depends on the selected metric and search type
	std::vector<Board>			children_;										// next states
	Board						*parent_;										// pointer to previous state
	address						zeroAddr_;										// coordinates of zero node
	uint64_t					hash_;											// hash sum of the table

	float						updateHeurstic_( const address& node,
												const int& metric,
												const Board& solution );		// update heuristic value according to the selected metric
	void						updateWeight_( const int& searchType );			// update weight according to the selected metric and search type
	void						swapNodes_( const address& newZero,
										   const int& metric,
										   const int& searchType,
										   const Board& solution );				// swap a node with the zero-node

public:
	Board( void );
	Board( const unsigned char& size );
	Board( const Board *rhs );

	void								generateCorrect( void );				// generate solution board
	void								generateRandom( void );					// generate random initial board if no input file provided

	unsigned char						getSize( void ) const;					// get length of side
	float								getWeight( void ) const;
	uint64_t							getHash( void ) const;
	int									getCost(void ) const;
	const Board							*getParent( void ) const;
	std::vector<Board>					&getChildren( void );
	std::vector<unsigned char>			*getMap( void );
	const std::vector<unsigned char>	*getMap( void ) const;
	void								setZeroAddress( void );
	void								setWeight( const int& metric,
												  const int& searchType,
												  const Board& solution );
	void								setHash( void );
	void								setChildren( const int& metric,
													const int& searchType,
													const Board& solution );

};

extern		std::ostream& operator<<( std::ostream& os, const Board& board );
extern		std::istream& operator>>( std::istream& is, Board& board );

#endif
