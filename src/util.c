/****************************************************************************
 * utility/util.h
 *
 *   Copyright (C) 2006-2010 Jetway Security Micro. All rights reserved.
 *   Author: Kenichi Cui <kenichi@jetsec.com.cn>
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "util.h"


/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * public  Functions
 ****************************************************************************/

void* memcpy(void *des, void *src, int size)
{
	unsigned char *d;
	unsigned char *s;
	
	if (des == src)
	{
		return des;
	}
	
	d = (unsigned char *)des;
	s = (unsigned char *)src;
	for (int i = 0; i < size; i++)
	{
		d[i] = s[i];
	}
	return des;
}

void* memset(void *src, unsigned int value, int size)
{
	unsigned char *s;
	
	s = (unsigned char *)src;
	for (int i = 0; i < size; i++)
	{
		s[i] = (unsigned char)value;
	}
	return src;
}















