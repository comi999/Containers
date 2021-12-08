#pragma once

template < typename ValueType >
class CReference;

template < typename ValueType >
class Reference
{
public:

	Reference()
		: m_Value( nullptr )
	{
	}

	Reference( ValueType& a_Value )
		: m_Value( &a_Value )
	{
	}

	Reference( ValueType&& ) = delete;

	Reference( Reference< ValueType >& a_Reference )
		: m_Value( a_Reference.GetPtr() )
	{
	}

	ValueType& Get()
	{
		return *m_Value;
	}

	const ValueType& Get() const
	{
		return *m_Value;
	}

	ValueType* GetPtr()
	{
		return m_Value;
	}

	const ValueType* GetPtr() const
	{
		return m_Value;
	}

	CReference< ValueType >& AsConst()
	{
		return *reinterpret_cast< CReference< ValueType >* >( this );
	}

	const CReference< ValueType >& AsConst() const
	{
		return *reinterpret_cast< const CReference< ValueType >* >( this );
	}

	inline bool IsSet() const
	{
		return m_Value;
	}

	inline operator ValueType& ( )
	{
		return *m_Value;
	}

	inline operator const ValueType& ( ) const
	{
		return *m_Value;
	}

private:

	ValueType* m_Value;

};

template < typename ValueType >
class CReference
{
public:

	CReference()
		: m_Value( nullptr )
	{
	}

	CReference( const ValueType& a_Value )
		: m_Value( &a_Value )
	{
	}

	CReference( ValueType&& ) = delete;

	CReference( const Reference< ValueType >& a_Reference )
		: m_Value( a_Reference.GetPtr() )
	{
	}

	CReference( const CReference< ValueType >& a_Reference )
		: m_Value( a_Reference.GetPtr() )
	{
	}

	const ValueType& Get()
	{
		return *m_Value;
	}

	const ValueType& Get() const
	{
		return *m_Value;
	}

	const ValueType* GetPtr()
	{
		return m_Value;
	}

	const ValueType* GetPtr() const
	{
		return m_Value;
	}

	inline bool IsSet() const
	{
		return m_Value;
	}

	inline operator const ValueType& ( ) const
	{
		return *m_Value;
	}

private:

	const ValueType* m_Value;

};
