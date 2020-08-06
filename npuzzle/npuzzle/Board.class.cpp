/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.class.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 09:21:41 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/06 18:32:01 by pacovali      ########   odam.nl         */
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
	distance_ = 0;
	parent_ = nullptr;
}

Board::Board( const size_t& size ) : size_( size ) {
	cost_ = 0;
	distance_ = 0;
	parent_ = nullptr;
	if (size < 3)
		throw logic_error( "Board side must be in range [3-16]" );
}

Board::Board( const Board& rhs ) {
	size_ = rhs.size_;
	pvals_ = rhs.pvals_;
	vvals_ = rhs.vvals_;
	cost_ = rhs.cost_;
	distance_ = rhs.distance_;
	children_ = rhs.children_;
	parent_ = nullptr;
}

/*
** *****************************************************************************
** OPERATORS
** *****************************************************************************
*/

void	Board::operator=( const Board& rhs ) {
	size_ = rhs.size_;
	pvals_ = rhs.pvals_;
	vvals_ = rhs.vvals_;
	cost_ = rhs.cost_;
	distance_ = rhs.distance_;
	children_ = rhs.children_;
	parent_ = rhs.parent_;
}

bool	Board::operator==( const Board& rhs ) const {
	return ( equal(pvals_.begin(), pvals_.end(), rhs.pvals_.begin()) );
}

bool	Board::operator!=( const Board& rhs ) const {
	return ( !equal(pvals_.begin(), pvals_.end(), rhs.pvals_.begin()) );
}

bool	Board::operator<( const Board& rhs ) const {
	return ( cost_ < rhs.cost_ && distance_ < rhs.distance_  );
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

bool	Board::generateCorrect( void ) {
	int	x_upper = (int)size_, y_upper = (int)size_;
	int	x_lower = -1, y_lower = -1;
	int	x_iter = 1, y_iter = 1;
	int	counter = 0;

	while (counter < (int)size_ * (int)size_) {
		for ( int i = x_lower + x_iter; i != x_upper; i += x_iter ) {
			vvals_[(counter + 1) % (size_ * size_)] = { y_lower + y_iter, i };
			pvals_[{ y_lower + y_iter, i }] = (counter + 1) % (size_ * size_);
			counter++;
		}
		swap_values( x_lower, x_upper, x_iter );
		y_lower += y_iter;
		for ( int i = y_lower + y_iter; i != y_upper; i += y_iter ) {
			vvals_[(counter + 1) % (size_ * size_)] = { i, x_lower + x_iter };
			pvals_[{ i, x_lower + x_iter }] = (counter + 1) % (size_ * size_);
			counter++;
		}
		x_lower += x_iter;
		swap_values( y_lower, y_upper, y_iter );
	}
	return ( true );
}

bool		Board::generateRandom( void ) {
	vector<int>		vals(size_ * size_);
	
	for (size_t i = 0; i < size_ * size_; i++) {
		vals[i] = (int)i;
	}
	shuffle(vals.begin(), vals.end(), std::default_random_engine(1));
	for (size_t i = 0; i < size_ * size_; i++) {
		vvals_[vals[i]] = { i / size_, i % size_ };
		pvals_[{ i / size_, i % size_ }] = vals[i];
	}
	return ( true );
}

bool		Board::isUnique( void ) {
	Board *tmp = parent_;
	while ( tmp ) {
		if ( tmp->vvals_ == vvals_ )
			return ( false );
		tmp = tmp->parent_;
	}
	return ( true );
}

size_t		Board::getSize( void ) const {
	return ( size_ );
}

std::map<short, std::pair<short, short>>&	Board::getAddressByValue( void ) {
	return ( vvals_ );
}

const std::map<short, std::pair<short, short>>&	Board::getAddressByValue( void ) const {
	return ( vvals_ );
}

std::map<std::pair<short, short>, short>&	Board::getValueByAddress( void ) {
	return ( pvals_ );
}

const std::map<std::pair<short, short>, short>&	Board::getValueByAddress( void ) const {
	return ( pvals_ );
}

int			Board::getCost( void ) const {
	return ( cost_ );
}

float		Board::getDistance( void ) const {
	return ( distance_ );
}

const Board	*Board::getParent( void ) const {
	return ( parent_ );
}

std::vector<Board>&	Board::getChildren( void ) {
	return ( children_ );
}

std::pair<int, int>	Board::getNodeAddress(short& val) const {
	return ( vvals_.at(val) );
}

short		Board::getNodeValue(const std::pair<int, int>& address) const {
	return ( pvals_.at(address) );
}

void		Board::setChildren( const int& metric, const Board& solution, Board* parent ) {
	pair<short, short>	zeroAddress = vvals_.at(0);
	if (zeroAddress.first > 0) {
		Board tmp(*this);
		tmp.swap_({zeroAddress.first - 1, zeroAddress.second}, zeroAddress, metric, solution);
		if (tmp.isUnique()) {
			children_.push_back(tmp);
		}
	}
	if (zeroAddress.first < int(size_ - 1)) {
		Board tmp(*this);
		tmp.swap_({zeroAddress.first + 1, zeroAddress.second}, zeroAddress, metric, solution);
		if (tmp.isUnique()) {
			children_.push_back(tmp);
		}
	}
	if (zeroAddress.second > 0) {
		Board tmp(*this);
		tmp.swap_({zeroAddress.first, zeroAddress.second - 1}, zeroAddress, metric, solution);
		if (tmp.isUnique()) {
			children_.push_back(tmp);
		}
	}
	if (zeroAddress.second < int(size_ - 1)) {
		Board tmp(*this);
		tmp.swap_({zeroAddress.first, zeroAddress.second + 1}, zeroAddress, metric, solution);
		if (tmp.isUnique()) {
			children_.push_back(tmp);
		}
	}
	for (auto& i : children_) {
		i.parent_ = parent;
		i.cost_++;
	}
}

void		Board::setDistance( const int& metric, const Board& solution ) {
	for (size_t row = 0; row < size_; row++) {
		for (size_t index = 0; index < size_; index++) {
			distance_ += solveMetrics_( *this, solution, metric, {row, index} );
		}
	}
}

/*
** *****************************************************************************
** PRIVATE METHODS
** *****************************************************************************
*/

float		solveManhattan(const pair<short, short>& curAddress, const pair<short, short>& solAddress) {
	return (abs(curAddress.first - solAddress.first) +
			abs(curAddress.second - solAddress.second));
}

float		solveEuclidean(const pair<short, short>& curAddress, const pair<short, short>& solAddress) {
	return (sqrt((curAddress.first - solAddress.first) *
			(curAddress.first - solAddress.first) +
			(curAddress.second - solAddress.second) *
			(curAddress.second - solAddress.second)));
}

float		solveHamming(short current, short solved) {
	return (current != solved);
}

float		Board::solveMetrics_(Board& current, const Board&  solution, int metric, const pair<short, short>& curAddress) {
	short curValue = current.getValueByAddress().at(curAddress);
	pair<short, short> solAddress = solution.getAddressByValue().at(curValue);

	if (metric == MANHATTAN) {
		return ( solveManhattan(curAddress, solAddress) );
	}
	if (metric == EUCLIDEAN) {
		return ( solveEuclidean(curAddress, solAddress) );
	}
	if (metric == HAMMING) {
		return ( solveHamming(curValue, solution.getValueByAddress().at(curAddress)) );
	}
	throw runtime_error("Unknown metric id at solveMetrics_");
	return (0);
}

void	Board::swap_(const std::pair<short, short>& valueAddress,
					 const std::pair<short, short>& zeroAddress,
					 int metric, const Board&  solution) {
	distance_ -= solveMetrics_(*this, solution, metric, valueAddress);
	short value = pvals_.at(valueAddress);
	pvals_[valueAddress] = 0;
	pvals_[zeroAddress] = value;
	vvals_[0] = valueAddress;
	vvals_[value] = zeroAddress;
	distance_ += solveMetrics_(*this, solution, metric, zeroAddress);
}

/*
** *****************************************************************************
** INPUT OUTPUT
** *****************************************************************************
*/

std::ostream& operator<<( std::ostream& stream, const Board& board ) {
	const map<pair<short,short>, short>& out = board.getValueByAddress();
	for (size_t i = 0; i < board.getSize(); i++) {
		for (size_t j = 0; j < board.getSize(); j++) {
			stream << setw(4) << out.at({ i, j }) << (j == board.getSize() - 1 ? "\n" : " ");
		}
	}
	return ( stream );
}

std::istream& operator>>( std::istream& stream, Board& board ) {
	map<pair<short,short>, short>& pvals = board.getValueByAddress();
	map<short, pair<short,short>>& vvals = board.getAddressByValue();

	
	for (size_t row = 0; row < board.getSize(); row++) {
		for (size_t index = 0; index < board.getSize(); index++) {
			short		tmp;
			stream >> tmp;
			if ( !stream || stream.fail() ) {
				if ( index != 0 ) {
					throw std::runtime_error( "Non-digit found" );
				} else {
					stream.clear();
					while ( stream.peek() == '#' ) {
						stream.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
					}
					stream >> tmp;
				}
			}
			if (vvals.count(tmp)) {
				throw runtime_error("Duplicate node found on board");
			}
			vvals.insert( {tmp, {row, index}} );
			pvals.insert( {{row, index}, tmp} );
		}
	}
	return ( stream );
}
