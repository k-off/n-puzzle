/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.class.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 09:21:41 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/01 09:21:51 by pacovali      ########   odam.nl         */
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
	size_ = 0;
	cost_ = 0;
	weight_ = 0;
	parent_ = nullptr;
	zeroAddr_ = {0, 0};
	hash_ = 0;
}

Board::Board( const unsigned char& size ) {
	size_ = size;
	cost_ = 0;
	weight_ = 0;
	parent_ = nullptr;
	zeroAddr_ = {0, 0};
	hash_ = 0;
	vals_.resize(size * size);
	if (size < 3 || size > 16)
		throw logic_error( "Board side must be in range (3-16)" );
}

Board::Board( const Board& rhs ) {
	size_ = rhs.size_;
	vals_ = rhs.vals_;
	cost_ = rhs.cost_ + 1;
	weight_ = rhs.weight_;
	children_ = rhs.children_;
	parent_ = (Board*)&rhs;
	zeroAddr_ = rhs.zeroAddr_;
}

/*
** *****************************************************************************
** OPERATORS
** *****************************************************************************
*/

void	Board::operator=( const Board& rhs ) {
	size_ = rhs.size_;
	vals_ = rhs.vals_;
	weight_ = rhs.weight_;
	parent_ = rhs.parent_;
	children_ = rhs.children_;
	zeroAddr_ = rhs.zeroAddr_;
}

bool	Board::operator==( const Board& rhs ) const {
	return ( hash_ == rhs.hash_ );
}

bool	Board::operator!=( const Board& rhs ) const {
	return ( hash_ != rhs.hash_ );
}

bool	Board::operator<( const Board& rhs ) const {
	return ( weight_ < rhs.weight_ );
}

/*
** *****************************************************************************
** PUBLIC MEMBER FUNCTIONS
** *****************************************************************************
*/

//helper function for correct generator
static void	swap_values( int& first, int& second, int& iter ) {
	int tmp;

	tmp = first;
	first = second;
	second = tmp;
	iter = -iter;
}

void	Board::generateCorrect( void ) {
	int	x_upper = (int)size_, y_upper = (int)size_;
	int	x_lower = -1, y_lower = -1;
	int	x_iter = 1, y_iter = 1;
	int	counter = 0;

	while (counter < (int)size_ * (int)size_) {
		for ( int column = x_lower + x_iter; column != x_upper; column += x_iter ) {
			int		row = (y_lower + y_iter);
			vals_[row * size_ + column ] = (counter + 1) % (size_ * size_);
			counter++;
		}
		swap_values( x_lower, x_upper, x_iter );
		y_lower += y_iter;
		for ( int row = (y_lower + y_iter); row != y_upper; row += y_iter ) {
			int column = x_lower + x_iter;
			vals_[row * size_ + column] = (counter + 1) % (size_ * size_);
			counter++;
		}
		x_lower += x_iter;
		swap_values( y_lower, y_upper, y_iter );
	}
	auto zero = find(vals_.begin(), vals_.end(), 0);
	unsigned char distance = zero - vals_.begin();
	zeroAddr_ = { (char)(distance % size_), (char)(distance / size_) };
	setHash();
}

void		Board::generateRandom( void ) {
	vals_ = vector<unsigned char>(size_ * size_);
	
	for (size_t i = 0; i < size_ * size_; i++) {
		vals_[i] = (int)i;
	}
	shuffle(vals_.begin(), vals_.end(), std::default_random_engine((int)time(nullptr)));
	auto zero = find(vals_.begin(), vals_.end(), 0);
	unsigned char distance = zero - vals_.begin();
	zeroAddr_ = { (char)(distance % size_), (char)(distance / size_) };
	setHash();
}

vector<unsigned char> *Board::getMap( void ) {
	return ( &vals_ );
}

const vector<unsigned char>	*Board::getMap( void ) const {
	return ( &vals_ );
}

unsigned char		Board::getSize( void ) const {
	return ( size_ );
}

float		Board::getWeight( void ) const {
	return ( weight_ );
}

const Board	*Board::getParent( void ) const {
	return ( parent_ );
}


std::vector<Board>	*Board::getChildren( void ) {
	return ( &children_ );
}

void		Board::setChildren( const int& metric, const int& searchType, const Board& solution ) {
	if ( zeroAddr_.x > 0 ) {
		children_.push_back(*this);
		children_[children_.size() - 1].swapNodes_({(char)(zeroAddr_.x - 1), zeroAddr_.y}, metric, searchType, solution);
	}
	if ( zeroAddr_.x < (size_ - 1) ) {
		children_.push_back(*this);
		children_[children_.size() - 1].swapNodes_({(char)(zeroAddr_.x + 1), zeroAddr_.y}, metric, searchType, solution);
	}
	if ( zeroAddr_.y > 0 ) {
		children_.push_back(*this);
		children_[children_.size() - 1].swapNodes_({zeroAddr_.x, (char)(zeroAddr_.y - 1)}, metric, searchType, solution);
	}
	if ( zeroAddr_.y < (size_ - 1) ) {
		children_.push_back(*this);
		children_[children_.size() - 1].swapNodes_({zeroAddr_.x, (char)(zeroAddr_.y + 1)}, metric, searchType, solution);
	}
}

void	Board::setHash( void ) {
	char			resultOffset = 0;
	char			currentOffset = ( size_ == 3 ? 8 : 4 );				// for boards 3*3 don't compress data as it fits 64bits
	
	for ( uint64_t current : vals_ ) {
		while ( current ) {
			hash_ ^= ( (current) << (resultOffset & 63) );
			current >>= currentOffset;
			resultOffset += currentOffset;
		}
	}
}

void	Board::setWeight( const int& metric, const int& searchType, const Board& solution ) {
	if ( searchType == GREEDY || searchType == STANDARD ) {
		for ( char i = 0; i < size_ * size_; i++ ) {
			address currentAddress( {char(i % size_), char(i / size_)} );
			updateHeurstic_(currentAddress, metric, solution);
		}
	} else {
		heur_ = 0;
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
	auto	value = vals_[node.x + node.y * size_];

	if ( metric == MANHATTAN ) {
		res = find(solution.vals_.begin(), solution.vals_.end(), value) - solution.vals_.begin();
	} else if ( metric == EUCLIDEAN ) {
		int x = (int)( find(solution.vals_.begin(), solution.vals_.end(), value) - solution.vals_.begin() ) % size_;
		int y = (int)( find(solution.vals_.begin(), solution.vals_.end(), value) - solution.vals_.begin() ) / size_;
		res = sqrt( x * x + y * y );
	} else {
		res = vals_[node.x  + node.y * size_] == solution.vals_[node.x  + node.y * size_];
	}
	return (res);
}

void	Board::updateWeight_( const int& searchType ) {
	if ( searchType == COST_UNIFORM ) {
		weight_ = cost_;
	} else if ( searchType == GREEDY ) {
		weight_ = heur_;
	} else {
		weight_ = heur_ + cost_;
	}
}

void	Board::swapNodes_( const address& newZero, const int& metric, const int& searchType, const Board& solution ) {
	address curZero = zeroAddr_;
	cost_ -= updateHeurstic_( newZero, metric, solution );
	cost_ -= updateHeurstic_( curZero, metric, solution );
	vals_[zeroAddr_.x + zeroAddr_.y * size_] = vals_[newZero.x + newZero.y * size_];
	vals_[newZero.x + newZero.y * size_] = 0;
	cost_ += updateHeurstic_( newZero, metric, solution );
	cost_ += updateHeurstic_( curZero, metric, solution );
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
	for (unsigned char i = 0; i < board.getSize() * board.getSize(); i++) {
		stream << setw(4) << int(out[0][i]) << ((i + 1) % board.getSize() ? " " : "\n");
	}
	return ( stream );
}

std::istream& operator>>( std::istream& stream, Board& board ) {
	vector<unsigned char> *in = (vector<unsigned char>*)board.getMap();
	set<unsigned char> check;
	
	for (unsigned char i = 0; i < board.getSize() * board.getSize(); i++) {
		short		tmp;
		stream >> tmp;
		if ( !stream || stream.fail() ) {
			if ( (i + 1) % board.getSize() || stream.peek() != '#' ) {						// if non-digit in the middle of line
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
	return ( stream );
}
