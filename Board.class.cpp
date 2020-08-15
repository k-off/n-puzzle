/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.class.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 09:21:41 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/09 15:48:33 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Board.class.hpp"

using namespace std;

/*
** *****************************************************************************
** CONSTRUCTORS
** *****************************************************************************
*/

Board::Board ( void ) {
	sideLength_ = 0;
	heuristic_ = 0;
	cost_ = 0;
	weight_ = 0;
	parent_ = nullptr;
	zeroAddr_ = {0, 0};
	hash_ = 0;
	children_ = {};
}

Board::Board( const unsigned char& size ) {
	if (size < 3 || size > 16)
		throw logic_error( "Board side must be in range (3-16)" );
	sideLength_ = size;
	board_.resize(sideLength_ * sideLength_);
	heuristic_ = 0;
	cost_ = 0;
	weight_ = 0;
	parent_ = nullptr;
	zeroAddr_ = {0, 0};
	hash_ = 0;
	children_ = {};
}

Board::Board( const Board *rhs ) {
	sideLength_ = rhs->sideLength_;
	board_ = rhs->board_;
	heuristic_ = rhs->heuristic_;
	cost_ = rhs->cost_ + 1;
	weight_ = rhs->weight_;
	parent_ = (Board*)rhs;
	zeroAddr_ = rhs->zeroAddr_;
	hash_ = rhs->hash_;
	children_ = {};
}

/*
** *****************************************************************************
** PUBLIC MEMBER FUNCTIONS
** *****************************************************************************
*/

static void	swap_values( int& first, int& second, int& iter ) {
	int tmp;

	tmp = first;
	first = second;
	second = tmp;
	iter = -iter;
}

void	Board::generateCorrect( void ) {
	int	x_upper = (int)sideLength_, y_upper = (int)sideLength_;
	int	x_lower = -1, y_lower = -1;
	int	x_iter = 1, y_iter = 1;
	int	counter = 0;

	while (counter < (int)sideLength_ * (int)sideLength_) {
		for ( int column = x_lower + x_iter; column != x_upper; column += x_iter ) {
			int		row = (y_lower + y_iter);
			board_[row * sideLength_ + column ] = (counter + 1) % (sideLength_ * sideLength_);
			counter++;
		}
		swap_values( x_lower, x_upper, x_iter );
		y_lower += y_iter;
		for ( int row = (y_lower + y_iter); row != y_upper; row += y_iter ) {
			int column = x_lower + x_iter;
			board_[row * sideLength_ + column] = (counter + 1) % (sideLength_ * sideLength_);
			counter++;
		}
		x_lower += x_iter;
		swap_values( y_lower, y_upper, y_iter );
	}
	auto zero = find(board_.begin(), board_.end(), 0);
	unsigned char distance = zero - board_.begin();
	zeroAddr_ = { (char)(distance % sideLength_), (char)(distance / sideLength_) };
	setHash();
}

void		Board::generateRandom( void ) {
	board_ = vector<unsigned char>(sideLength_ * sideLength_);
	
	for (size_t i = 0; i < sideLength_ * sideLength_; i++) {
		board_[i] = (int)i;
	}
	shuffle(board_.begin(), board_.end(), std::default_random_engine((int)time(nullptr)));
	auto zero = find(board_.begin(), board_.end(), 0);
	unsigned char distance = zero - board_.begin();
	zeroAddr_ = { (char)(distance % sideLength_), (char)(distance / sideLength_) };
	setHash();
}

vector<unsigned char> *Board::getMap( void ) {
	return ( &board_ );
}

const vector<unsigned char>	*Board::getMap( void ) const {
	return ( &board_ );
}

unsigned char		Board::getSize( void ) const {
	return ( sideLength_ );
}

float		Board::getWeight( void ) const {
	return ( weight_ );
}

const Board	*Board::getParent( void ) const {
	return ( parent_ );
}


std::vector<Board>	&Board::getChildren( void ) {
	return ( children_ );
}

void		Board::setChildren( const int& metric, const int& searchType, const Board& solution ) {
	if ( zeroAddr_.x > 0 ) {
		children_.push_back(this);
		children_[children_.size() - 1].swapNodes_({(char)(zeroAddr_.x - 1), zeroAddr_.y}, metric, searchType, solution);
	}
	if ( zeroAddr_.x < (sideLength_ - 1) ) {
		children_.push_back(this);
		children_[children_.size() - 1].swapNodes_({(char)(zeroAddr_.x + 1), zeroAddr_.y}, metric, searchType, solution);
	}
	if ( zeroAddr_.y > 0 ) {
		children_.push_back(this);
		children_[children_.size() - 1].swapNodes_({zeroAddr_.x, (char)(zeroAddr_.y - 1)}, metric, searchType, solution);
	}
	if ( zeroAddr_.y < (sideLength_ - 1) ) {
		children_.push_back(this);
		children_[children_.size() - 1].swapNodes_({zeroAddr_.x, (char)(zeroAddr_.y + 1)}, metric, searchType, solution);
	}
}

void	Board::setZeroAddress( void ) {
	auto zero = find(board_.begin(), board_.end(), 0);
	unsigned char distance = zero - board_.begin();
	zeroAddr_ = { (char)(distance % sideLength_), (char)(distance / sideLength_) };
	setHash();
}

void	Board::setHash( void ) {
	char			resultOffset = 0;
	char			currentOffset = (sideLength_ < 9 ? 8 : 4);

	hash_ = 0;
	for ( uint64_t current : board_ ) {
		if (!current)
			resultOffset += 3;
		while ( current ) {
			hash_ ^= ( (current) << (resultOffset) );
			resultOffset &= 63;
			current >>= currentOffset;
			resultOffset += currentOffset;
		}
	}
}

void	Board::setWeight( const int& metric, const int& searchType, const Board& solution ) {
	if ( searchType == GREEDY || searchType == STANDARD ) {
		for ( int i = 0; i < sideLength_ * sideLength_; i++ ) {
			address currentAddress( {char(i % sideLength_), char(i / sideLength_)} );
			heuristic_ += updateHeurstic_(currentAddress, metric, solution);
		}
	} else {
		heuristic_ = 0;
	}
	updateWeight_( searchType );
}

uint64_t	Board::getHash( void ) const {
	return ( hash_ );
}

int			Board::getCost(void ) const {
	return ( cost_ );
}

/*
** *****************************************************************************
** PRIVATE METHODS
** *****************************************************************************
*/

float	Board::updateHeurstic_( const address& node, const int& metric, const Board& solution ) {
	float	res = 0;
	float	solutionPos = 0;
	float	currentPos = node.x + node.y * sideLength_;
	auto	value = board_[node.x + node.y * sideLength_];

	if ( metric == MANHATTAN ) {
		solutionPos = find(solution.board_.begin(), solution.board_.end(), value) - solution.board_.begin();
		int x = (int)( solutionPos - currentPos ) % sideLength_;
		int y = (int)( solutionPos - currentPos ) / sideLength_;
		res = x + y;
	} else if ( metric == EUCLIDEAN ) {
		solutionPos = find(solution.board_.begin(), solution.board_.end(), value) - solution.board_.begin();
		int x = (int)( solutionPos - currentPos ) % sideLength_;
		int y = (int)( solutionPos - currentPos ) / sideLength_;
		res = sqrt( x * x + y * y );
	} else {
		res = board_[node.x  + node.y * sideLength_] == solution.board_[node.x  + node.y * sideLength_];
	}
	return ( res < 0 ? -res : res );
}

void	Board::updateWeight_( const int& searchType ) {
	if ( searchType == COST_UNIFORM ) {
		weight_ = cost_;
	} else if ( searchType == GREEDY ) {
		weight_ = heuristic_;
	} else {
		weight_ = heuristic_ + cost_;
	}
}

void	Board::swapNodes_( const address& newZero, const int& metric, const int& searchType, const Board& solution ) {
	address curZero = zeroAddr_;
	heuristic_ -= updateHeurstic_( newZero, metric, solution );
	heuristic_ -= updateHeurstic_( curZero, metric, solution );
	board_[zeroAddr_.x + zeroAddr_.y * sideLength_] = board_[newZero.x + newZero.y * sideLength_];
	board_[newZero.x + newZero.y * sideLength_] = 0;
	heuristic_ += updateHeurstic_( newZero, metric, solution );
	heuristic_ += updateHeurstic_( curZero, metric, solution );
	zeroAddr_ = newZero;
	setHash();
	updateWeight_( searchType );
}

/*
** *****************************************************************************
** INPUT OUTPUT
** *****************************************************************************
*/

std::ostream& operator<<( std::ostream& stream, const Board& board ) {
	vector<unsigned char> *out = (vector<unsigned char>*)board.getMap();
	for (int i = 0; i < board.getSize() * board.getSize(); i++) {
		stream << setw(4) << int(out[0][i]) << ((i + 1) % board.getSize() ? " " : "\n");
	}
	return ( stream );
}

std::istream& operator>>( std::istream& stream, Board& board ) {
	vector<unsigned char> *in = (vector<unsigned char>*)board.getMap();
	set<unsigned char> check;

	for (int i = 0; i < board.getSize() * board.getSize(); i++) {
		short		tmp;
		stream >> tmp;
		if ( !stream || stream.fail() ) {
			if ( (i + 1) % board.getSize() || stream.peek() != '#' ) {
				throw std::runtime_error( "Non-digit found" );
			} else {
				stream.clear();
				while ( stream.peek() == '#' ) {
					stream.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
				}
				stream >> tmp;
			}
		}
		if (check.count(tmp)) {
			throw runtime_error("Duplicate node found on board");
		} else {
			check.insert(tmp);
		}
		if ( tmp < 0 ) {
			throw out_of_range("Negative node found");
		}
		if ( tmp > (board.getSize() * board.getSize() - 1) ) {
			throw out_of_range("Node value too big");
		}
		in[0][i] = tmp;
	}
	board.setZeroAddress();
	return ( stream );
}
