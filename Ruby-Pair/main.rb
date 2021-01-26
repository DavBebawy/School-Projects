#Global method cons
def cons(value1, value2)
  Pair.new(value1, value2)
end

#Implement pair? method for all Object class instances.
#Returns true if class Pair
class Object
  def pair?
    if is_a? Pair
      true
    else
      false
    end
  end
end

#Implement list? and null? method for NilClass instances.
#Returns true
class NilClass
  def list?
    true
  end
  def null?
    true
	end
	
	#Implement to_s method for NilClass instances.
	#Returns "()"
	def to_s
		"()"
	end
end

class Pair
	#null method, retuns nil
  def self.null
    nil
  end

	#initialization method, sets the first and second value
  def initialize(value1, value2)
    @value1 = value1
    @value2 = value2
  end

	#car method, returns the first value
  def car
    @value1
  end

	#cdr method, returns the second value
  def cdr
    @value2
  end

	#to_s method, formats the output to be the same as a Racket instruction
  def to_s
    if @value2.nil?
      format('(%s)', @value1.to_s)
    elsif @value2.is_a? Pair
      var = @value2.to_s
      var[0] = ''
      var[var.length - 1] = '' if var[var.length - 1] == ')'
      format('(%s %s)', @value1.to_s, var)
    else
      format('(%s . %s)', @value1.to_s, @value2.to_s)
    end
  end

	#list? method, returns true, if empty list, or a pair whose second element is a list.
  def list?
    if @value2.pair?
      if @value2.cdr.is_a?(Pair) || @value2.cdr.nil?
        true
      else
        false
      end
    else
      false
    end
  end

	#count method, returns the number of items in the top level of the list if the pair is a list, else returns false
  def count
    if pair?
      if @value2.pair?
        countsofar = @value2.count
        1 + countsofar
      elsif @value2.nil?
        1
      else
        2
      end
    elsif nil?
      0
    else
      1
    end
  end

  def setcdr(item)
    @value2 = item
  end

	#append method, returns a new list consisting of other appended to the original list, if the pair is a list. else, returns false.
  def append(other)
    if list?
      temp = Marshal.load(Marshal.dump(self))
      appendHelper(temp, other)
      temp
    else
      false
    end
  end

	#appendHelper method, used to recursively append other to a list
  def appendHelper(temp, appended)
    if temp.cdr.is_a? Pair
      appendHelper(temp.cdr, appended)
    elsif temp.cdr.nil?
      temp.setcdr(appended)
    else
      temp.setcdr(cons(temp.cdr, appended))
    end
	end
end